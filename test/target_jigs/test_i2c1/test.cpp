#include <unity.h>
#include "main.h"
#include <cube.hpp>
#include <i2c1.hpp>
#include <gpio.hpp>
#include <stm32l4xx_hal_msp.c>
#include <stm32l4xx_it.c>

I2C_HandleTypeDef hi2c1;

I2C_HandleTypeDef hi2c3;
UART_HandleTypeDef huart1;
LPTIM_HandleTypeDef hlptim1;
SPI_HandleTypeDef hspi1;

// This test requires an EXTERNAL BME680 sensor present in the QWIIC connector = I2C1
// TODO: next step is to test the I2C as slave, this will also test the related interrupt routings

void setUp(void) {        // before each test
}
void tearDown(void) {        // after each test
}

void test_i2c1() {
    TEST_ASSERT_TRUE((HAL_OK == HAL_I2C_IsDeviceReady(&hi2c1, 0x76 << 1, 3, 100)));
}

int main(int argc, char** argv) {
    HAL_Init();
    SystemClock_Config();
    HAL_Delay(2000);
    i2c1::wakeUp();
    UNITY_BEGIN();
    RUN_TEST(test_i2c1);
    UNITY_END();
}