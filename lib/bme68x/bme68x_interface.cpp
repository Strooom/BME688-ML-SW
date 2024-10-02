#include <bme68x_interface.hpp>
#include "stm32l4xx_hal.h"

#ifndef generic
#include "main.h"
extern SPI_HandleTypeDef hspi1;
#endif


BME68X_INTF_RET_TYPE bme68xInterface::read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    uint8_t device_addr = *(uint8_t *)intf_ptr;

    (void)intf_ptr;

    return HAL_SPI_TransmitReceive(&hspi1, &reg_addr, reg_data, 1, len, 500);
}

BME68X_INTF_RET_TYPE bme68xInterface::write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    uint8_t device_addr = *(uint8_t *)intf_ptr;

    (void)intf_ptr;

    return HAL_SPI_TransmitReceive(&hspi1, &reg_addr, reg_data, 1, len, 500);
}

void bme68xInterface::delay(uint32_t period, void *intf_ptr) {
    (void)intf_ptr;
    HAL_Delay(period);
}
