/**********************************************************
 *  INCLUDES
 *********************************************************/
#include <gtest/gtest.h>
#include <unistd.h>
#include <stdio.h>

extern "C" {
#include "arduino_code.h"
}

/**********************************************************
 *  Test: get_temperature
 *********************************************************/

TEST(test_get_temperature, temp_fetch) 
{ 
    // To be executed on 13/10/2022 10:45 GMT
    // The set previous time temperature is 13/10/2022 10:42 GMT
    time_temperature = 1665657720000.0;
    temperature = 0.0;
    sunlight_on = 1;
    heater_on = 1;
    get_temperature();
    // The expect near precision is the temperature change in 30"
    EXPECT_NEAR(temperature, 1000, 167);
}


TEST(test_exec_cmd_msg, basic) 
{ 
    // test 1
    last_cmd_msg.cmd = 1;
    last_cmd_msg.set_heater = 1;
    exec_cmd_msg();
    ASSERT_EQ(1, heater_on);
    ASSERT_EQ(1, next_res_msg.cmd);
    ASSERT_EQ(1, next_res_msg.status);
    ASSERT_EQ(0, last_cmd_msg.cmd);
    ASSERT_EQ(0, last_cmd_msg.set_heater);

    // test 2
    last_cmd_msg.cmd = 2;
    sunlight_on = 1;
    exec_cmd_msg();
    ASSERT_EQ(1, next_res_msg.data.sunlight_on);
    ASSERT_EQ(2, next_res_msg.cmd);
    ASSERT_EQ(1, next_res_msg.status);
    ASSERT_EQ(0, last_cmd_msg.cmd);
    ASSERT_EQ(0, last_cmd_msg.set_heater);

    // test 3
    last_cmd_msg.cmd = 3;
    temperature = 10;
    exec_cmd_msg();
    ASSERT_EQ(10, next_res_msg.data.temperature);
    ASSERT_EQ(3, next_res_msg.cmd);
    ASSERT_EQ(1, next_res_msg.status);
    ASSERT_EQ(0, last_cmd_msg.cmd);
    ASSERT_EQ(0, last_cmd_msg.set_heater);

    // test 4
    last_cmd_msg.cmd = 4;
    position = {2427.050983124840, 3526.711513754840, 9708.203932499370};
    exec_cmd_msg();
    EXPECT_NEAR(2427.050983124840, next_res_msg.data.position.x, 0.001);
    EXPECT_NEAR(3526.711513754840, next_res_msg.data.position.y, 0.001);
    EXPECT_NEAR(9708.203932499370, next_res_msg.data.position.z, 0.001);
    
    ASSERT_EQ(4, next_res_msg.cmd);
    ASSERT_EQ(1, next_res_msg.status);
    ASSERT_EQ(0, last_cmd_msg.cmd);
    ASSERT_EQ(0, last_cmd_msg.set_heater);
}

int main(int argc, char **argv) 
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

