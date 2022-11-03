// Include files
// --------------------------------------
#include <string.h>
#include <stdio.h>
#include <time.h>

//  -------------------------------------
//  Define Pins
//  -------------------------------------

int led = 13;
int ldr = A3;

//  -------------------------------------
//  Aux variables
//  -------------------------------------

unsigned long int SC = 0;
double starttime;
double endtime;
double deltatime;

// --------------------------------------
// CONSTANTS
// --------------------------------------

# define SHIP_SPECIFIC_HEAT 0.9
# define SHIP_MASS 10.0  // Kg
# define HEATER_POWER 150.0 // J/sec
# define SUNLIGHT_POWER 50.0 // J/sec
# define HEAT_POWER_LOSS -100.0 // J/sec

# define ORBIT_POINTS_SIZE 20
# define ORBIT_TIME 300.0  //sec

// --------------------------------------
// Types
// --------------------------------------

// structture of a position on the orbit
struct position {
    float x;
    float y;
    float z;
};

// list of commands to be send
enum command {NO_CMD=0, SET_HEAT_CMD=1, READ_SUN_CMD=2, 
        READ_TEMP_CMD=3, READ_POS_CMD=4};

// structure of command message
struct cmd_msg {
    short int cmd;  // command to execute
    short int set_heater;     // boolean to set or unset the heater
};

// structure of response message
struct res_msg {
    short int cmd;  // command to respond to
    short int status;        // boolean to state if execution went well
    union {
      short int sunlight_on;   // boolean to state if sunlight is on
      float temperature;   // value of the temperature
      struct position position;   // value of the position
   } data;
};

// --------------------------------------
// PUBLIC STATUS (GLOBAL VARIABLES)
// --------------------------------------

// boolean with the status of the heater
int heater_on = 0;
// boolean with the status of the sunlight
int sunlight_on = 0;
// Save the actual temperature of the ship
double temperature = 0.0;
// save the last time temperature was computed 
double time_temperature = 0.0;
// inital time of the orbit
double init_time_orbit = 0.0;
// actual position of the ship
struct position position = {0.0, 0.0, 0.0};

bool command_in_process = false;
bool response_ready = false;

// last command message received
struct cmd_msg last_cmd_msg = {NO_CMD, 0};
// next response message to be send
struct res_msg next_res_msg = {NO_CMD, 0};

// --------------------------------------
// PRIVATE STATUS (STATIC GLOBAL VARIABLES)
// --------------------------------------

// position data for the orbit
static struct position orbit_points[ORBIT_POINTS_SIZE] = {
    {3000.00, 0, 12000.0},
    {2853.169548885460, 1854.101966249680, 11412.678195541800},
    {2427.050983124840, 3526.711513754840, 9708.203932499370},
    {1763.355756877420, 4854.101966249680, 7053.423027509680},
    {927.050983124842, 5706.339097770920, 3708.203932499370},
    {0.0, 6000.0, 0.0},
    {-927.050983124842, 5706.339097770920, -3708.203932499370},
    {-1763.355756877420, 4854.101966249680, -7053.423027509680},
    {-2427.050983124840, 3526.711513754840, -9708.203932499370},
    {-2853.169548885460, 1854.101966249680, -11412.678195541800},
    {-3000.0, 0.0, -12000.0},
    {-2853.169548885460, -1854.101966249690, -11412.678195541800},
    {-2427.050983124840, -3526.711513754840, -9708.203932499370},
    {-1763.355756877420, -4854.101966249680, -7053.423027509680},
    {-927.050983124843, -5706.339097770920, -3708.203932499370},
    {0.0, -6000.0, 0.0},
    {927.050983124842, -5706.339097770920, 3708.203932499370},
    {1763.355756877420, -4854.101966249690, 7053.423027509680},
    {2427.050983124840, -3526.711513754840, 9708.203932499370},
    {2853.169548885460, -1854.101966249690, 11412.678195541800}};

// Command message parity
unsigned char cmd_parity = (unsigned char) 0;

//---------------------------------------------------------------------------
//                           AUXILIAR FUNCTIONS 
//---------------------------------------------------------------------------

// --------------------------------------
// Function: getClock
// --------------------------------------
double getClock()
{
    return ((double) millis()) / 1000.0;
}

// --------------------------------------
// Function: delayClock
// --------------------------------------
void delayClock(double time)
{
    delay(time * 1000.0);
}

//---------------------------------------------------------------------------
//                           MAIN FUNCTIONS
//---------------------------------------------------------------------------
// --------------------------------------
// Function: comm_server
// --------------------------------------
int comm_server()
{
   static int count = 0;
   unsigned char car_aux;

   // If there were a received msg, send the processed answer or ERROR if none.
   // then reset for the next request.
   // NOTE: this requires that between two calls of com_server all possible 
   //       answers have been processed.
   if (command_in_process) {
      // if there is an answer send it, else error
      if (!response_ready) {
         next_res_msg.cmd = NO_CMD;//////////////////////
         next_res_msg.status = 1;
      }  
      // compute parity
      unsigned char res_parity = (unsigned char) 0;
      for (int i=0; i<sizeof(struct res_msg); i++) {
         // compute parity (xor)
         res_parity = res_parity ^ ((unsigned char *)(&next_res_msg))[i];  
      } 
      Serial.write((unsigned char *)(&next_res_msg), sizeof(struct res_msg));
      Serial.write((unsigned char *)(&res_parity), 1);
      // reset flags and buffers
      command_in_process = false;/////////////////////////////////////////////////////////
      response_ready = false;///////////////////////
      memset((unsigned char *)(&last_cmd_msg), 0, sizeof(struct cmd_msg));
      memset((unsigned char *)(&next_res_msg), 0, sizeof(struct res_msg));
   }

   while (Serial.available()) {
      // read one character
      car_aux =Serial.read();
      if ((count == 0) && (car_aux > 4)) {
         continue;
      }
      // Check if it is the last byte of the msg or not.
      if (count == sizeof(struct cmd_msg)) {
         // check parity error
         if (cmd_parity != car_aux) {
            // set error answer
            last_cmd_msg.cmd = NO_CMD;
            last_cmd_msg.set_heater = 0;
            command_in_process = true;//////////////////////////////////////////////////
            next_res_msg.cmd = NO_CMD;
            next_res_msg.status = 2;
            response_ready = true;/////////////////////////////
            cmd_parity = (unsigned char) 0;
            count = 0;
            // end loop
            break;
         }
         // finish reading msg
         cmd_parity = (unsigned char) 0;
         count = 0;
         command_in_process = true;//////////////////////////////////////
         
         break;
      } else {
         //Store the character
         ((unsigned char *) (&last_cmd_msg))[count] = car_aux;
         // compute parity (xor)
         cmd_parity = cmd_parity ^ car_aux;
      }

      // Increment the count
      count++;
   }
}

/**********************************************************
 *  Function: get_temperature
 *********************************************************/
void get_temperature ()
{
  double previous_time_temperature = time_temperature;
  time_temperature = getClock();
  double dtimetemp = time_temperature - previous_time_temperature;
  double power = ((sunlight_on * SUNLIGHT_POWER) + (heater_on * HEATER_POWER)) + HEAT_POWER_LOSS;
  double energy = power * dtimetemp;
  temperature = (energy / (SHIP_SPECIFIC_HEAT * SHIP_MASS)) + temperature;
}

/**********************************************************
 *  Function: get_position
 *********************************************************/

void get_position ()
{
  double dtimeorbit = fmod((getClock() - init_time_orbit), ORBIT_TIME);
  double dorbit = (dtimeorbit / ORBIT_TIME);
  double pos_index = dorbit * ORBIT_POINTS_SIZE;
  int pos_index_low = (int) pos_index;
  double offset_ratio = pos_index - pos_index_low;

  position.x = (orbit_points[pos_index_low-1].x * (1 - offset_ratio)) + (orbit_points[pos_index_low].x * (offset_ratio));
  position.y = (orbit_points[pos_index_low-1].y * (1 - offset_ratio)) + (orbit_points[pos_index_low].y * (offset_ratio));
  position.z = (orbit_points[pos_index_low-1].z * (1 - offset_ratio)) + (orbit_points[pos_index_low].z * (offset_ratio));
}

/**********************************************************
 *  Function: exec_cmd_msg
 *********************************************************/
void exec_cmd_msg ()
{
  if (command_in_process == true)
  {
	  if (last_cmd_msg.cmd == NO_CMD){
	  	  next_res_msg.cmd = NO_CMD;
		} else if (last_cmd_msg.cmd == SET_HEAT_CMD) {
		  heater_on = last_cmd_msg.set_heater;
		  next_res_msg.cmd = SET_HEAT_CMD;
		  next_res_msg.status = 1;
		} else if (last_cmd_msg.cmd == READ_SUN_CMD) {
		  next_res_msg.data.sunlight_on = sunlight_on;
		  next_res_msg.cmd = READ_SUN_CMD;
		  next_res_msg.status = 1;
		} else if (last_cmd_msg.cmd == READ_TEMP_CMD) {
		  next_res_msg.data.temperature = temperature;
		  next_res_msg.cmd = READ_TEMP_CMD;
		  next_res_msg.status = 1;
		} else if (last_cmd_msg.cmd == READ_POS_CMD) {
		  next_res_msg.data.position = position;
		  next_res_msg.cmd = READ_POS_CMD;
		  next_res_msg.status = 1;
		}
		last_cmd_msg.cmd = NO_CMD;
		last_cmd_msg.set_heater = 0;
		response_ready = true;
  }
}        

/**********************************************************
 *  Function: read_sun_sensor
 *********************************************************/
void read_sun_sensor ()
{
  if (analogRead(ldr) > 512) {
    sunlight_on = 1;
  } else {
    sunlight_on = 0;
  }
}

/**********************************************************
 *  Function: set_heater
 *********************************************************/
void set_heater ()
{
  digitalWrite(led, heater_on);
}

// --------------------------------------
// Function: setup
// --------------------------------------
void setup()
{
  // Setup Serial Monitor
  Serial.begin(9600);
  // Declare pin mode
  pinMode(led, OUTPUT);
  starttime = getClock();
}

// --------------------------------------
// Function: loop
// --------------------------------------
void loop()
{
  switch(SC) {
    case 0:
      set_heater();
      read_sun_sensor();
      get_position();
      get_temperature();
    case 1:
      set_heater();
      read_sun_sensor();
      comm_server();
     case 2:
      set_heater();
      read_sun_sensor();
      get_position();
      get_temperature();
     case 3:
      set_heater();
      read_sun_sensor();
      exec_cmd_msg();
  }

  SC = (SC + 1) % 4;
  endtime = getClock();
  deltatime = endtime - starttime;
  starttime = starttime + 0.05;
  if (deltatime > 0.05) {
    analogWrite(led, 50);
  } else {
    delayClock(0.05 - deltatime + 0.003);
  }
}
