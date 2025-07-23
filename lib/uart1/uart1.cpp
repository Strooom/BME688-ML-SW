// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include <uart1.hpp>
#include <gpio.hpp>

#ifndef generic
#include "main.h"
extern UART_HandleTypeDef huart1;
#endif

bool uart1::initalized{false};

void uart1::wakeUp() {
    if (!initalized) {
#ifndef generic
        initialize();
#endif
        gpio::enableGpio(gpio::group::uart1);
        initalized = true;
    }
}

void uart1::goSleep() {
    if (initalized) {
#ifndef generic
        HAL_UART_DeInit(&huart1);
#endif
        gpio::disableGpio(gpio::group::uart1);
        initalized = false;
    }
}

void uart1::initialize() {
    huart1.Instance                    = USART1;
    huart1.Init.BaudRate               = 115200;
    huart1.Init.WordLength             = UART_WORDLENGTH_8B;
    huart1.Init.StopBits               = UART_STOPBITS_1;
    huart1.Init.Parity                 = UART_PARITY_NONE;
    huart1.Init.Mode                   = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
}