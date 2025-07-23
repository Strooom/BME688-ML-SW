#pragma once
#include <main.h>

void delay_us(uint32_t period, void *intf_ptr) {
#ifndef generic
    HAL_Delay(period / 1000);
#endif
}