#include <main.h>
#include <stm32l4xx_hal_msp.c>
#include <stm32l4xx_it.c>
#include <cstring>
#include <clock.hpp>
#include <gpio.hpp>
#include <uart1.hpp>
#include <i2c1.hpp>
#include <i2c3.hpp>
#include <delay.hpp>

#include <bme68x_defs.h>
#include <bme68x.h>
#include <bsec_interface.h>

#include <unity.h>

// This test will check some properties of the bsec library, to verify it is linked to the build

void setUp(void) {}
void tearDown(void) {}

void test_getVersion() {
    bsec_version_t bsecVersion;

    bsec_get_version(nullptr, &bsecVersion);
    TEST_ASSERT_EQUAL(3, bsecVersion.major);
    TEST_ASSERT_EQUAL(2, bsecVersion.minor);
    TEST_ASSERT_EQUAL(1, bsecVersion.major_bugfix);
    TEST_ASSERT_EQUAL(0, bsecVersion.minor_bugfix);
}

void test_getInstanceSize() {
    TEST_ASSERT_EQUAL(1384, bsec_get_instance_size());
}

int main(int argc, char** argv) {
    HAL_Init();
    SystemClock_Config();
    HAL_Delay(1000);

    i2c3::wakeUp();
    UNITY_BEGIN();
    RUN_TEST(test_getVersion);
    RUN_TEST(test_getInstanceSize);
    UNITY_END();
}