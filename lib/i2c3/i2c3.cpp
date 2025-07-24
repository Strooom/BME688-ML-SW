// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include <i2c3.hpp>
#include <gpio.hpp>

#ifndef generic
#include "main.h"
I2C_HandleTypeDef hi2c3;
#endif

bool i2c3::initalized{false};

void i2c3::wakeUp() {
    if (!initalized) {
#ifndef generic
        initialize();
#endif
        gpio::enableGpio(gpio::group::i2c3);
        initalized = true;
    }
}

void i2c3::goSleep() {
    if (initalized) {
#ifndef generic
        HAL_I2C_DeInit(&hi2c3);
#endif
        gpio::disableGpio(gpio::group::i2c3);
        initalized = false;
    }
}

void i2c3::initialize() {
    hi2c3.Instance              = I2C3;
    hi2c3.Init.Timing           = 0x00B07CB4;
    hi2c3.Init.OwnAddress1      = 0;
    hi2c3.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    hi2c3.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    hi2c3.Init.OwnAddress2      = 0;
    hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c3.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    hi2c3.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c3) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK) {
        Error_Handler();
    }
}

BME68X_INTF_RET_TYPE i2c3::read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    uint8_t device_addr = *(uint8_t *)intf_ptr;
    HAL_StatusTypeDef status;
    (void)intf_ptr;

    status = HAL_I2C_Mem_Read(&hi2c3, device_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)reg_data, len, HAL_MAX_DELAY);

    if (status == HAL_OK) {
        return BME68X_INTF_RET_SUCCESS; // TODO : strange, both success and fail return 0
    } else {
        return 0;
    }
}


BME68X_INTF_RET_TYPE i2c3::write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    uint8_t device_addr = *(uint8_t*)intf_ptr;
    HAL_StatusTypeDef status;
    (void)intf_ptr;
    
    status = HAL_I2C_Mem_Write(&hi2c3, device_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)reg_data, len, HAL_MAX_DELAY);

    if (status == HAL_OK) {
        return BME68X_INTF_RET_SUCCESS;
    } 
    else{
        return 0;
    }   
}
