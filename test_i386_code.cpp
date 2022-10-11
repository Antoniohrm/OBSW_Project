/**********************************************************
 *  INCLUDES
 *********************************************************/
#include <gtest/gtest.h>
#include <unistd.h>
#include <stdio.h>

extern "C" {
#include "i386_code.h"
}

/**********************************************************
 *  Test: control_temperature -> basic
 *********************************************************/

TEST(test_control_temperature, basic) 
{ 
    // test 1
    temperature = 20;
    control_temperature();
    ASSERT_EQ(1, heater_on);

    // test 1
    temperature = 60;
    control_temperature();
    ASSERT_EQ(0, heater_on);
}

/**********************************************************
 *  Test: send_cmd_msg -> heater
 *********************************************************/
// in progress, to be modified!!! 
TEST(test_send_cmd_msg, heater) 
{ 
    // test 1
    cmd = SET_HEAT_CMD;
    //send_cmd_msg();
    ASSERT_EQ(heater_on, send_cmd_msg(SET_HEAT_CMD, 0));
}

// what about for heater off -> heater_on=0 ??



/**********************************************************
 *  Test: recv_res_msg -> errors
 *********************************************************/

TEST(test_recv_res_msg, errors) 
{ 
    // test 1
    last_res_msg.cmd = SET_HEAT_CMD;
    last_res_msg.status = 0;
    //recv_res_msg();
    ASSERT_EQ(<ERROR>, recv_res_msg(SET_HEAT_CMD, 0));
    

    // test 2
    last_res_msg.cmd == READ_SUN_CMD;
    last_res_msg.status = 0;
    //recv_res_msg();
    ASSERT_EQ(<ERROR>, recv_res_msg(READ_SUN_CMD, 0));

    // test 3
    last_res_msg.cmd == READ_TEMP_CMD;
    last_res_msg.status = 0;
    //recv_res_msg();
    ASSERT_EQ(<ERROR>, recv_res_msg(READ_TEMP_CMD, 0));

    // test 4
    last_res_msg.cmd == READ_POS_CMD;
    last_res_msg.status = 0;
    //recv_res_msg();
    ASSERT_EQ(<ERROR>, recv_res_msg(READ_POS_CMD, 0));
    
}



/**********************************************************
 *  Funtion: main
 *********************************************************/

int main(int argc, char **argv) 
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

