#include <unity.h>
#include "main.h"
#include <cube.hpp>
#include <stm32l4xx_hal_msp.c>
#include <stm32l4xx_it.c>
#include <gpio.hpp>
#include <sdcard.hpp>

I2C_HandleTypeDef hi2c3;
UART_HandleTypeDef huart1;

I2C_HandleTypeDef hi2c1;
LPTIM_HandleTypeDef hlptim1;
SPI_HandleTypeDef hspi1;

void setUp(void) {}
void tearDown(void) {}

void dummy_test_message() {
    TEST_IGNORE_MESSAGE("Card detect test - Insert, then remove card, then insert again");
}

void test_card_detect() {
    static constexpr uint32_t testTimeOut{5000};
    gpio::enableGpio(gpio::group::spiSdCard);

    TEST_ASSERT_FALSE_MESSAGE(sdCard::isPresent(), "Card detected at test start");
    for (uint32_t loops = 0; loops < testTimeOut; loops++) {
        if (sdCard::isPresent()) {
            TEST_MESSAGE("Card detected");
            break;
        }
        HAL_Delay(1);
    }
    TEST_ASSERT_TRUE_MESSAGE(sdCard::isPresent(), "Card detection timeout");
    for (uint32_t loops = 0; loops < testTimeOut; loops++) {
        if (!sdCard::isPresent()) {
            TEST_MESSAGE("Card removal detected");
            break;
        }
        HAL_Delay(1);
    }
    TEST_ASSERT_FALSE_MESSAGE(sdCard::isPresent(), "Card removal timeout");
    for (uint32_t loops = 0; loops < testTimeOut; loops++) {
        if (sdCard::isPresent()) {
            TEST_MESSAGE("Card detected");
            break;
        }
        HAL_Delay(1);
    }
    TEST_ASSERT_TRUE_MESSAGE(sdCard::isPresent(), "Card detection timeout");
}

void test_read_write() {
    TEST_IGNORE_MESSAGE("Test not implemented yet");
}

int main(int argc, char** argv) {
    HAL_Init();
    HAL_Delay(2000);
    SystemClock_Config();
    UNITY_BEGIN();
    RUN_TEST(dummy_test_message);
    RUN_TEST(test_card_detect);
    RUN_TEST(test_read_write);
    UNITY_END();
}