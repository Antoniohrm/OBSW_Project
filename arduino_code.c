/********************
 *  INCLUDES
 *******************/

#include <stdio.h>
#include <time.h>
#include <math.h>

#include "arduino_code.h"

/********************
 *  CONSTANTS
 *******************/

#define NS_PER_S  1000000000

#define SHIP_SPECIFIC_HEAT 0.9
#define SHIP_MASS 10.0  // Kg
#define HEATER_POWER 150.0 // J/sec
#define SUNLIGHT_POWER 50.0 // J/sec
#define HEAT_POWER_LOSS -100.0 // J/sec

#define ORBIT_POINTS_SIZE 20
#define ORBIT_TIME 300.0  //sec

/********************
 *  PUBLIC STATUS (GLOBAL VARIABLES)
 ********************/

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
struct position position;

// last command message received
struct cmd_msg last_cmd_msg = {NO_CMD, 0};
// next response message to be send
struct res_msg next_res_msg = {NO_CMD, 0};

/********************
 *  PRIVATE STATUS (STATIC GLOBAL VARIABLES)
 ********************/

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

//---------------------------------------------------------------------------
//                           AUXILIAR FUNCTIONS 
//---------------------------------------------------------------------------

/********************
 *  Function: getClock
 *******************/
double getClock()
{
    struct timespec tp;
    double reloj;

    clock_gettime (CLOCK_REALTIME, &tp);
    reloj = ((double)tp.tv_sec) +
	    ((double)tp.tv_nsec) / ((double)NS_PER_S);

    return (reloj);
}

//---------------------------------------------------------------------------
//                           MAIN FUNCTIONS
//---------------------------------------------------------------------------

/********************
 *  Function: get_temperature
 *******************/
void get_temperature ()
{
	double previous_time_temperature = time_temperature;
	time_temperature = getClock();
	double dtimetemp = time_temperature - previous_time_temperature;
	double power = ((sunlight_on * SUNLIGHT_POWER) + (heater_on * HEATER_POWER)) + HEAT_POWER_LOSS;
	double energy = power * dtimetemp;
	temperature = (energy / (SHIP_SPECIFIC_HEAT * SHIP_MASS)) + temperature;
}

/********************
 *  Function: get_position
 *******************/

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

/********************
 *  Function: exec_cmd_msg
 *******************/

void exec_cmd_msg ()
{
	if (last_cmd_msg.cmd == 1) {
        	heater_on = last_cmd_msg.set_heater;
        	next_res_msg.cmd = 1;
        	next_res_msg.status = 1;
        } else if (last_cmd_msg.cmd == 2) {
        	next_res_msg.data.sunlight_on = sunlight_on;
        	next_res_msg.cmd = 2;
        	next_res_msg.status = 1;
        } else if (last_cmd_msg.cmd == 3) {
        	next_res_msg.data.temperature = temperature;
        	next_res_msg.cmd = 3;
        	next_res_msg.status = 1;
        } else if (last_cmd_msg.cmd == 4) {
        	next_res_msg.data.position = position;
        	next_res_msg.cmd = 4;
        	next_res_msg.status = 1;
        }
        last_cmd_msg.cmd = 0;
        last_cmd_msg.set_heater = 0;
}

