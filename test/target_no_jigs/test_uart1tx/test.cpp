#include <unity.h>
#include "main.h"
#include <cube.hpp>
#include <circularbuffer.hpp>
#include <stm32l4xx_hal_msp.c>
#include <stm32l4xx_it.cpp>


void setUp(void) {        // before each test
}
void tearDown(void) {        // after each test
}

void test_spare_IOs() {
    gpio::enableDisableGpio(gpio::group::other, true);
    for (uint32_t i = 0; i < 16; i++) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
        HAL_Delay(1000);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
        HAL_Delay(1000);
    }
}

int main(int argc, char** argv) {
    HAL_Init();
    HAL_Delay(2000);
    SystemClock_Config();
    MX_USART1_UART_Init();

    UNITY_BEGIN();
    RUN_TEST(test_spare_IOs);
    UNITY_END();
}