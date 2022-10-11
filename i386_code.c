/********************
 *  INCLUDES
 *******************/

#include <stdio.h>
#include <time.h>

#include "i386_code.h"

/********************
 *  CONSTANTS
 *******************/

#define MAX_TEMPERATURE 90.
#define MIN_TEMPERATURE -10.0 
#define AVG_TEMPERATURE 40.0 

/********************
 *  PUBLIC STATUS (GLOBAL VARIABLES)
 ********************/

// boolean with the status of the heater
int heater_on = 0;
// boolean with the status of the sunlight
int sunlight_on = 0;
// Save the actual temperature of the ship
double temperature = 0.0;
// actual position of the ship
struct position position;

// next command message to be send
struct cmd_msg next_cmd_msg = {NO_CMD, 0};
// last response message received
struct res_msg last_res_msg = {NO_CMD, 0};

//---------------------------------------------------------------------------
//                           MAIN FUNCTIONS
//---------------------------------------------------------------------------


/********************
 *  Function: control_temperature
 *******************/
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

/********************
 *  Function: send_cmd_msg
 *******************/
void send_cmd_msg (enum command cmd)
{
	next_cmd_msg.cmd = cmd;
	if (cmd == SET_HEAT_CMD) {
		next_cmd_msg.set_heater = heater_on;
	}
}

/********************
 *  Function: recv_res_msg
 *******************/
void recv_res_msg ()
{
	if (last_res_msg.cmd == SET_HEAT_CMD) {
		if (last_res_msg.status != 1) {<ERROR>}
	} else if (last_res_msg.cmd == READ_SUN_CMD) {
		sunlight_on = last_res_msg.data.sunlight_on;
		if (last_res_msg.status != 1) {<ERROR>}
	} else if (last_res_msg.cmd == READ_TEMP_CMD) {
		temperature = last_res_msg.data.temperature;
		if (last_res_msg.status != 1) {<ERROR>}
	} else if (last_res_msg.cmd == READ_POS_CMD) {
		position = last_res_msg.data.position;
		if (last_res_msg.status != 1) {<ERROR>}
	}
	last_res_msg = {0, 0};
}
