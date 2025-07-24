#include <main.h>
#include <stm32l4xx_hal_msp.c>
#include <stm32l4xx_it.c>
#include <cstring>
#include <clock.hpp>
#include <gpio.hpp>
#include <uart1.hpp>

#include <i2c3.hpp>
#include <delay.hpp>

#include <bme68x_defs.h>
#include <bme68x.h>
#include <bsec_interface.h>

#include <unity.h>

I2C_HandleTypeDef hi2c3;
UART_HandleTypeDef huart1;
I2C_HandleTypeDef hi2c1;
LPTIM_HandleTypeDef hlptim1;

// This test will initialize both BME68X sensors on the PCB, using the Bosch BME68X library

void setUp(void) {}
void tearDown(void) {}

void test_initialize() {
#define NUM_OF_SENS 2
    uint8_t* bsecInstance[NUM_OF_SENS];
    bsec_version_t bsecVersion;
    bsec_library_return_t getVersionResult;
    getVersionResult = bsec_get_version(bsecInstance, &bsecVersion);
    TEST_ASSERT_EQUAL(3, bsecVersion.major);
    TEST_ASSERT_EQUAL(2, bsecVersion.minor);
    TEST_ASSERT_EQUAL(1, bsecVersion.major_bugfix);
    TEST_ASSERT_EQUAL(0, bsecVersion.minor_bugfix);
}

int main(int argc, char** argv) {
    HAL_Init();
    SystemClock_Config();
    HAL_Delay(1000);

    i2c3::wakeUp();
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    UNITY_END();
}