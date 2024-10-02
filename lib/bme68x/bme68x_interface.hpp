#pragma once

#include "bme68x_defs.h"
#include <stdint.h>

class bme68xInterface {
  public:
    static BME68X_INTF_RET_TYPE read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
    static BME68X_INTF_RET_TYPE write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);
    static void delay(uint32_t delayInMicroseconds, void *intf_ptr);
};

