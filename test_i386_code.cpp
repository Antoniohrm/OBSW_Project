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
 *  Funtion: main
 *********************************************************/

int main(int argc, char **argv) 
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

