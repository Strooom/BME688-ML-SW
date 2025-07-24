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

#include <unity.h>

I2C_HandleTypeDef hi2c3;
UART_HandleTypeDef huart1;
I2C_HandleTypeDef hi2c1;
LPTIM_HandleTypeDef hlptim1;

// This test will initialize both BME68X sensors on the PCB, using the Bosch BME68X library

void setUp(void) {}
void tearDown(void) {}

void test_initialize() {
    struct bme68x_dev sensor0;
    memset(&sensor0, 0, sizeof(sensor0));
    uint8_t sensor0I2cAddress = BME68X_I2C_ADDR_LOW;
    sensor0.intf              = BME68X_I2C_INTF;
    sensor0.read              = i2c3::read;
    sensor0.write             = i2c3::write;
    sensor0.delay_us          = delay_us;
    sensor0.intf_ptr          = &sensor0I2cAddress;
    sensor0.amb_temp          = 22;

    struct bme68x_dev sensor1;
    memset(&sensor1, 0, sizeof(sensor1));
    uint8_t sensor1I2cAddress = BME68X_I2C_ADDR_HIGH;
    sensor1.intf              = BME68X_I2C_INTF;
    sensor1.read              = i2c3::read;
    sensor1.write             = i2c3::write;
    sensor1.delay_us          = delay_us;
    sensor1.intf_ptr          = &sensor1I2cAddress;
    sensor1.amb_temp          = 22;

    int8_t initResult{BME68X_E_DEV_NOT_FOUND};
    initResult = bme68x_init(&sensor0);
    TEST_ASSERT_EQUAL_INT8(BME68X_OK, initResult);
    initResult = bme68x_init(&sensor1);
    TEST_ASSERT_EQUAL_INT8(BME68X_OK, initResult);
}

void test_selftest() {
    struct bme68x_dev sensor0;
    memset(&sensor0, 0, sizeof(sensor0));
    uint8_t sensor0I2cAddress = BME68X_I2C_ADDR_LOW;
    sensor0.intf              = BME68X_I2C_INTF;
    sensor0.read              = i2c3::read;
    sensor0.write             = i2c3::write;
    sensor0.delay_us          = delay_us;
    sensor0.intf_ptr          = &sensor0I2cAddress;
    sensor0.amb_temp          = 22;

    struct bme68x_dev sensor1;
    memset(&sensor1, 0, sizeof(sensor1));
    uint8_t sensor1I2cAddress = BME68X_I2C_ADDR_HIGH;
    sensor1.intf              = BME68X_I2C_INTF;
    sensor1.read              = i2c3::read;
    sensor1.write             = i2c3::write;
    sensor1.delay_us          = delay_us;
    sensor1.intf_ptr          = &sensor1I2cAddress;
    sensor1.amb_temp          = 22;

    int8_t selfTestResult = bme68x_selftest_check(&sensor0);
    TEST_ASSERT_EQUAL_INT8(BME68X_OK, selfTestResult);
    selfTestResult = bme68x_selftest_check(&sensor1);
    TEST_ASSERT_EQUAL_INT8(BME68X_OK, selfTestResult);
}


int main(int argc, char** argv) {
    HAL_Init();
    SystemClock_Config();
    HAL_Delay(1000);

    i2c3::wakeUp();
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_selftest);
    UNITY_END();
}