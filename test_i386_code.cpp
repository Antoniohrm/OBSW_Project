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
TEST(test_send_cmd_msg, heater) 
{ 
    // test 1
    enum command cmd = SET_HEAT_CMD;
    heater_on = 1;
    send_cmd_msg(cmd);
    ASSERT_EQ(1, next_cmd_msg.cmd);
    ASSERT_EQ(1, next_cmd_msg.set_heater);
}


/**********************************************************
 *  Test: recv_res_msg -> errors
 *********************************************************/

TEST(test_recv_res_msg, errors) 
{ 

    // test 1
    last_res_msg.cmd = 2;
    last_res_msg.data.sunlight_on = 1;
    recv_res_msg();

    ASSERT_EQ(1, sunlight_on);
    ASSERT_EQ(0, last_res_msg.status);
    ASSERT_EQ(0, last_res_msg.cmd);


    // test 2
    last_res_msg.cmd = 3;
    last_res_msg.data.temperature = 10;
    recv_res_msg();
    ASSERT_EQ(10, temperature);
    ASSERT_EQ(0, last_res_msg.status);
    ASSERT_EQ(0, last_res_msg.cmd);


    // test 3
    last_res_msg.cmd = 4;
    last_res_msg.data.position = {2427.050983124840, 3526.711513754840, 9708.203932499370};
    recv_res_msg();
    EXPECT_NEAR(2427.050983124840, position.x, 0.001);
    EXPECT_NEAR(3526.711513754840, position.y,0.001);
    EXPECT_NEAR(9708.203932499370, position.z,0.001);
    ASSERT_EQ(0, last_res_msg.status);
    ASSERT_EQ(0, last_res_msg.cmd);

	
    // test 4
    last_res_msg.cmd = 0;
    recv_res_msg();
    ASSERT_EQ(0, last_res_msg.cmd);
    ASSERT_EQ(0, last_res_msg.status);
	
    // test 5
    last_res_msg.cmd = 1;
    recv_res_msg();
    ASSERT_EQ(0, last_res_msg.cmd);
    ASSERT_EQ(0, last_res_msg.status);
}



/**********************************************************
 *  Funtion: main
 *********************************************************/

int main(int argc, char **argv) 
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
