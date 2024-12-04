#include <unity.h>
#include "main.h"
#include <cube.hpp>
#include <stm32l4xx_hal_msp.c>
#include <stm32l4xx_it.c>

// This test requires an I2C-based sensor present in the QWIIC connector
// TODO: next step is to test the I2C as slave, this will also test the related interrupt routings

void setUp(void) {        // before each test
}
void tearDown(void) {        // after each test
}

void test_i2c1() {
}

int main(int argc, char** argv) {
    HAL_Init();
    HAL_Delay(2000);
    SystemClock_Config();
    MX_USART1_UART_Init();

    UNITY_BEGIN();
    RUN_TEST(void test_i2c1());
    UNITY_END();
}