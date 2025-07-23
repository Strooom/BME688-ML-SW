// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include <i2c1.hpp>
#include <gpio.hpp>

#ifndef generic
#include "main.h"
extern I2C_HandleTypeDef hi2c1;

#endif

bool i2c1::initalized{false};

void i2c1::initialize() {
    hi2c1.Instance              = I2C1;
    hi2c1.Init.Timing           = 0x00B07CB4;
    hi2c1.Init.OwnAddress1      = 36;
    hi2c1.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2      = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
        Error_Handler();
    }
}

void i2c1::wakeUp() {
    if (!initalized) {
#ifndef generic
        i2c1::initialize();
#endif
        gpio::enableGpio(gpio::group::i2c1);
        initalized = true;
    }
}

void i2c1::goSleep() {
    if (initalized) {
#ifndef generic
        HAL_I2C_DeInit(&hi2c1);
#endif
        gpio::disableGpio(gpio::group::i2c1);
        initalized = false;
    }
}
