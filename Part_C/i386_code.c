//-Uncomment to compile with arduino support
#define ARDUINO

/**********************************************************
 *  INCLUDES
 *********************************************************/
#include <termios.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <time.h>
#include <sys/errno.h>
#include <sys/stat.h>


#include <rtems.h>
#include <rtems/termiostypes.h>
#include <bsp.h>

// --------------------------------------
// Constants
// --------------------------------------
#define SLAVE_ADDR 0x8
#define NS_PER_S  1000000000


#define MAX_TEMPERATURE 90.
#define MIN_TEMPERATURE -10.0
#define AVG_TEMPERATURE 40.0

//  -------------------------------------
//  Aux variables
//  -------------------------------------

unsigned long int SC = 0;
double starttime;
double endtime;
double deltatime;

// --------------------------------------
// Types
// --------------------------------------

// list of commands to be send
enum command {NO_CMD=0, SET_HEAT_CMD=1, READ_SUN_CMD=2,
              READ_TEMP_CMD=3, READ_POS_CMD=4};

// structture of a position on the orbit
struct position {
    float x;
    float y;
    float z;
};


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
// Global Variables
// --------------------------------------
// file descriptor for I2C
int file_desc;

// boolean with the status of the heater
int heater_on = 0;
// boolean with the status of the sunlight
int sunlight_on = 0;
// Save the actual temperature of the ship
double temperature = 0.0;
// actual position of the ship
struct position position = {0.0, 0.0, 0.0};

// next command message to be send
struct cmd_msg next_cmd_msg = {NO_CMD, 0};
// last response message received
struct res_msg last_res_msg = {NO_CMD, 0};

//---------------------------------------------------------------------------
//                           AUXILIAR FUNCTIONS
//---------------------------------------------------------------------------

// --------------------------------------
// Function: getClock
// --------------------------------------
double getClock()
{
    struct timespec tp;
    double reloj;

    clock_gettime (CLOCK_REALTIME, &tp);
    reloj = ((double)tp.tv_sec) +
        ((double)tp.tv_nsec) / ((double)NS_PER_S);

    return (reloj);
}

// --------------------------------------
// Function: delayClock
// --------------------------------------
void delayClock(double time)
{
    struct timespec sleep_time;

    //set double into struct timespec
    double floor_time = (double)((unsigned long) time);
    sleep_time.tv_sec = (unsigned long) time;
    sleep_time.tv_nsec = (unsigned long) ((time - floor_time)* (double)(NS_PER_S));

    // sleep
    nanosleep(&sleep_time,NULL);
}

// --------------------------------------
// Function: send_msg
// --------------------------------------
void send_msg ()
{

    // compute parity
    unsigned char res_parity = (unsigned char) 0;
    int ret = 0;

    for (int i=0; i<sizeof(struct cmd_msg); i++) {
       // compute parity (xor)
       res_parity = res_parity ^ ((unsigned char *)(&next_cmd_msg))[i];
    }

    // send message to slave
    ret = write(file_desc,(char *)&next_cmd_msg,sizeof(struct cmd_msg));
    if( ret < sizeof(struct cmd_msg)){
        printf("ERROR: write Request: ret=%d \n", ret);
        sleep(5);
        exit (-1);
    }
    // send parity to slave
    ret = write(file_desc,(char *)&res_parity,1);
    if(ret < 1){
        printf("ERROR: write parity: ret=%d \n", ret);
        sleep(5);
        exit (-1);
    }
}

// --------------------------------------
// Function: recv_msg
// --------------------------------------
void recv_msg ()
{

    // compute parity
    unsigned char res_parity = (unsigned char) 0;
    unsigned char parity = (unsigned char) 0;
    int ret = 0;

    // send message to slave
    int leido=0;
    while (leido<sizeof(struct res_msg)) {
        //printf("BEGIN: read Request: ret=%d \n", ret);

      ret = read(file_desc,((char *)(&last_res_msg))+leido,sizeof(struct res_msg)-leido);
      if( ret <= 0){
        printf("ERROR: read Request: ret=%d \n", ret);
        sleep(5);
        exit (-1);

      }
      //printf("read Request: ret=%d \n", ret);
      leido = leido +ret;

    }
    for (int i=0; i<sizeof(struct res_msg); i++) {
       // compute parity (xor)
       res_parity = res_parity ^ ((unsigned char *)(&last_res_msg))[i];
    }

    // send parity to slave
    ret = 0;
    while (ret <= 0) {
      ret = read(file_desc,(char *)&parity,1);
      if (ret < 1) {
        printf("ERROR: read parity: ret=%d \n", ret);
        sleep(5);
        exit (-1);
      }
      //printf("read Parity: ret=%d \n", ret);
    }
    if (parity != res_parity) {
        printf("ERROR: received wrong parity\n");
        last_res_msg.cmd = NO_CMD;
        last_res_msg.status = 2;
    }
}

// --------------------------------------
// Function: send_cmd_msg
// --------------------------------------
void send_cmd_msg (enum command cmd)
{
	next_cmd_msg.cmd = cmd;
			if (cmd == SET_HEAT_CMD) {
				next_cmd_msg.set_heater = heater_on;
			}
}

// --------------------------------------
// Function: recv_res_msg
// --------------------------------------
void recv_res_msg ()
{
	if (last_res_msg.status == 1) { // The command executed correctly
		if (last_res_msg.cmd == 2) {
				sunlight_on = last_res_msg.data.sunlight_on;
			} else if (last_res_msg.cmd == 3) {
				temperature = last_res_msg.data.temperature;
			} else if (last_res_msg.cmd == 4) {
				position = last_res_msg.data.position;
			}

			//last_res_msg.cmd = NO_CMD;
			//last_res_msg.status = NO_CMD;
	}
}

//---------------------------------------------------------------------------
//                           MAIN FUNCTIONS
//---------------------------------------------------------------------------

// --------------------------------------
// Function: control_temperature
// --------------------------------------
void control_temperature ()
{
    // check if temperature is lower or higher
    if (temperature < AVG_TEMPERATURE) {
    // set heater
    heater_on = 1;
    } else if (temperature >= AVG_TEMPERATURE) {
    // unset heater
    heater_on = 0;
    }
}



// --------------------------------------
// Function: print_state
// --------------------------------------
void print_state ()
{
	printf("Heater: %d\n",heater_on);
		printf("Sun Sensor: %d\n",sunlight_on);
		printf("Temperature: %f\n",temperature);
		printf("Position: %f, %f, %f\n\n",position.x,
	                                        position.y,
	                                        position.z);
}


// --------------------------------------
// Function: execute_cmd
// --------------------------------------
void execute_cmd (enum command cmd)
{
    // prepare request buffer
    send_cmd_msg (cmd);

#ifdef ARDUINO
    // send message to slave
    send_msg();
#endif

    // wait until answer is ready
    delayClock(0.400);

#ifdef ARDUINO
    //receive the answer from the slave
    recv_msg();
#endif

    //parse response
    recv_res_msg ();
}

//-------------------------------------
//-  Function: controller
//-------------------------------------
void *controller(void *arg)
{
	delayClock(2.0);
	starttime = getClock();
	    // Endless loop
	   while (1) {
		   switch(SC) {
		   case 0:
		   	    	execute_cmd (READ_TEMP_CMD);
		   	    	execute_cmd (SET_HEAT_CMD);
		   	    	execute_cmd (READ_SUN_CMD);

		   	    	control_temperature();
		   	    	break;
		   case 1:
		   	    	execute_cmd (READ_TEMP_CMD);
		   	    	execute_cmd (SET_HEAT_CMD);
		   	    	execute_cmd (READ_POS_CMD);
		            	control_temperature();
		            	print_state();
		            	break;
		  }
		  endtime = getClock();
		  SC = (SC + 1) % 2;
		  deltatime = endtime - starttime;	
		  starttime = starttime + 2;

		  delayClock(2.0 - deltatime);

	    }
}

//-------------------------------------
//-  Function: Init
//-------------------------------------
rtems_task Init (rtems_task_argument ignored)
{
    pthread_t thread_ctrl;
    sigset_t alarm_sig;
    int i;

    /* Block all real time signals so they can be used for the timers.
     Note: this has to be done in main() before any threads are created
     so they all inherit the same mask. Doing it later is subject to
     race conditions */
    sigemptyset (&alarm_sig);
    for (i = SIGRTMIN; i <= SIGRTMAX; i++) {
        sigaddset (&alarm_sig, i);
    }
    sigprocmask (SIG_BLOCK, &alarm_sig, NULL);

#if defined(ARDUINO)
    /* Open serial port */
    char serial_dev[]="/dev/com1";
    file_desc = open (serial_dev, O_RDWR);
    if (file_desc < 0) {
        printf("open: error opening serial %s\n", serial_dev);
        sleep(5);
        exit(-1);
    }

    struct termios portSettings;
    speed_t speed=B9600;

    tcgetattr(file_desc, &portSettings);
    cfsetispeed(&portSettings, speed);
    cfsetospeed(&portSettings, speed);
    cfmakeraw(&portSettings);
    tcsetattr(file_desc, TCSANOW, &portSettings);
#endif
    
    /* Create first thread */
    pthread_create(&thread_ctrl, NULL, controller, NULL);
    pthread_join (thread_ctrl, NULL);
    exit(0);
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS 1
#define CONFIGURE_MAXIMUM_SEMAPHORES 10
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 30
#define CONFIGURE_MAXIMUM_DIRVER 10
#define CONFIGURE_MAXIMUM_POSIX_THREADS 10
#define CONFIGURE_MAXIMUM_POSIX_TIMERS 10

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
