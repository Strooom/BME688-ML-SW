// ######################################################################################
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include <bme688.hpp>
#include <logging.hpp>
#include <i2c3.hpp>

#ifndef generic
#include "main.h"
extern I2C_HandleTypeDef hi2c3;
#else
uint8_t mockBME688Registers[256];
bool mockBME688Present{false};
#include <cstring>
#endif

bme688::bme688(const uint8_t i2cAddress) : i2cAddress(i2cAddress) {
}

bool bme688::isPresent() {
    // 1. Check if something is connected to the I2C bus at the address of the bme688
    if (!testI2cAddress()) {
        return false;
    }
    // 2. Check if it is a bme688 by reading the chip id register/value
    uint8_t chipidValue = readRegister(bme688::registers::chipId);
    return (bme688::chipIdValue == chipidValue);
}

bool bme688::testI2cAddress() {
#ifndef generic
    return (HAL_OK == HAL_I2C_IsDeviceReady(&hi2c3, i2cAddress << 1, halTrials, halTimeout));

#else
    return mockbme688Present;
#endif
}

uint8_t bme688::readRegister(registers registerAddress) {
    uint8_t result;
#ifndef generic
    HAL_I2C_Mem_Read(&hi2c3, i2cAddress << 1, static_cast<uint16_t>(registerAddress), I2C_MEMADD_SIZE_8BIT, &result, 1, halTimeout);
#else
    result = mockbme688Registers[static_cast<uint8_t>(registerAddress)];
#endif
    return result;
}
