#include <unity.h>
#include "main.h"
#include <cube.hpp>
#include <stm32l4xx_hal_msp.c>
#include <stm32l4xx_it.c>
#include <gpio.hpp>
#include <i2c3.hpp>        // on the BME688-ML-V1-PCB, the two BME688 sensors are connected to I2C3
#include <bme688.hpp>

I2C_HandleTypeDef hi2c3;
UART_HandleTypeDef huart1;

I2C_HandleTypeDef hi2c1;
LPTIM_HandleTypeDef hlptim1;
SPI_HandleTypeDef hspi1;

void setUp(void) {}
void tearDown(void) {}

bme688 sensor0(0x76);
bme688 sensor1(0x77);

void test_bme68x_presence_0() {
    TEST_ASSERT_TRUE(sensor0.isPresent());
}

void test_bme68x_presence_1() {
    TEST_ASSERT_TRUE(sensor1.isPresent());
}

int main(int argc, char** argv) {
    HAL_Init();
    SystemClock_Config();
    HAL_Delay(2000);
    i2c3::wakeUp();
    UNITY_BEGIN();
    RUN_TEST(test_bme68x_presence_0);
    RUN_TEST(test_bme68x_presence_1);
    UNITY_END();}