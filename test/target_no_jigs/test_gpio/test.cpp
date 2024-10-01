#include <unity.h>
#include "main.h"
#include <cube.hpp>
#include <gpio.hpp>
#include <sdcard.hpp>
#include <stm32l4xx_hal_msp.c>
#include <stm32l4xx_it.cpp>

void setUp(void) {
}
void tearDown(void) {
}

void test_cs_bme68x() {
    static constexpr uint32_t period{3000};
    TEST_MESSAGE("Measure voltage on CS-BME68X");
    gpio::enableGpio(gpio::group::spiSensor);
    for (uint32_t loops = 0; loops < 16; loops++) {
        HAL_GPIO_TogglePin(CS_SDCARD_GPIO_Port, CS_SDCARD_Pin);
        HAL_Delay(period);
    }
}

void test_cs_sdcard() {
    static constexpr uint32_t period{3000};
    TEST_MESSAGE("Measure voltage on CS-SDCard");
    gpio::enableGpio(gpio::group::spiSensor);
    for (uint32_t loops = 0; loops < 16; loops++) {
        HAL_GPIO_TogglePin(CS_BME688_GPIO_Port, CS_BME688_Pin);
        HAL_Delay(period);
    }
}

void test_card_detect() {
    static constexpr uint32_t testTimeOut{5000};
    TEST_MESSAGE("Card detect test - Insert, then remove card, then insert again");
    gpio::enableGpio(gpio::group::spiMemory);

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

int main(int argc, char** argv) {
    HAL_Init();
    HAL_Delay(2000);
    SystemClock_Config();
    UNITY_BEGIN();
    RUN_TEST(test_cs_bme68x);
    RUN_TEST(test_cs_sdcard);
    RUN_TEST(test_card_detect);
    UNITY_END();
}