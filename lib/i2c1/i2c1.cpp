// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include <i2c1.hpp>
#include <gpio.hpp>

#ifndef generic
#include "main.h"
extern I2C_HandleTypeDef hi2c1;
void MX_I2C1_Init(void);
#endif

bool i2c1::initalized{false};

void i2c1::wakeUp() {
    if (!initalized) {
#ifndef generic
        MX_I2C1_Init();
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
