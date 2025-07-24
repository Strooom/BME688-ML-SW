#include <main.h>
#include <stm32l4xx_hal_msp.c>
#include <stm32l4xx_it.c>
#include <clock.hpp>
#include <gpio.hpp>
#include <uart1.hpp>
#include <i2c1.hpp>
#include <i2c3.hpp>
#include <unity.h>

// UART1 is the test output port. So any testresults showing up confirms UART1 TX is working

void setUp(void) {}
void tearDown(void) {}
void test_tx() {}

int main(int argc, char** argv) {
    HAL_Init();
    SystemClock_Config();
    HAL_Delay(1000);

    UNITY_BEGIN();
    RUN_TEST(test_tx);
    UNITY_END();
}