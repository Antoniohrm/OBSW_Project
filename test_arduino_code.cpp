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
 
// To be executed on 13/10/2022 12:38:00 GMT
// The set previous time temperature is 13/10/2022 12:35:00 GMT
double ref_time = 1665664500.0;

TEST(test_get_temperature, temp_fetch) 
{ 
    // To be executed on 13/10/2022 11:42:15 GMT
    // The set previous time temperature is 13/10/2022 11:39:15 GMT
    
    // Test temperature change with heater and sunlight on
    time_temperature = ref_time;
    temperature = 0.0;
    sunlight_on = 1;
    heater_on = 1;
    get_temperature();
    // The expect near precision is the temperature change in 30"
    EXPECT_NEAR(temperature, 2000, 334);
    
    // Test temperature change with heater and sunlight off
    time_temperature = ref_time;
    temperature = 0.0;
    sunlight_on = 0;
    heater_on = 0;
    get_temperature();
    // The expect near precision is the temperature change in 30"
    EXPECT_NEAR(temperature, -2000, 334);
}

TEST(test_get_position, pos_fetch)
{
    // The expect near precisions are half of the max distance of the desired points to the closest,
    // what means around a 12.5 second margin in the test execution
    
    // The first position test uses a time in orbit of 180 sec, or 3/5ths of the orbit,
    // it makes for the 12th position exactly in the coordinates struct
    
    init_time_orbit = ref_time;
    get_position();
    EXPECT_NEAR(position.x, -2853.169548885460, 200);
    EXPECT_NEAR(position.y, -1854.101966249690, 900);
    EXPECT_NEAR(position.z, -11412.678195541800, 700);
    
    // The second position test uses a time in orbit of 170 sec, so it falls in the 11.34th
    // position of the coordinates struct
    
    init_time_orbit = ref_time + 10;
    get_position();
    EXPECT_NEAR(position.x, -2950.077646621060, 200);
    EXPECT_NEAR(position.y, -630.394668524895, 900);
    EXPECT_NEAR(position.z, -11800.310586484200, 700);
    
    // The third position test uses a time in orbit of 335 sec, so it falls in the 2.34th
    // position of the coordinates struct
    
    init_time_orbit = ref_time - 155;
    get_position();
    EXPECT_NEAR(position.x, 2708.289236526850, 200);
    EXPECT_NEAR(position.y, 2422.789212401430, 900);
    EXPECT_NEAR(position.z, 10833.156946107400, 700);
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

