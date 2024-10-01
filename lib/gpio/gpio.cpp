#include <gpio.hpp>

#ifndef generic
#include "main.h"
#else

#endif

void gpio::enableGpio(group aGroup) {
    enableDisableGpio(aGroup, true);
}

void gpio::disableGpio(group aGroup) {
    enableDisableGpio(aGroup, false);
}

void gpio::disableAllGpio() {
    enableDisableGpio(gpio::group::spiSensor, false);
    enableDisableGpio(gpio::group::spiMemory, false);
    enableDisableGpio(gpio::group::i2c, false);
    enableDisableGpio(gpio::group::uart1, false);
    // NOTE : debugPort is not disabled as it is used for logging
}

void gpio::enableDisableGpio(group theGroup, bool enable) {
#ifndef generic
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    switch (theGroup) {
        case gpio::group::i2c:
            if (enable) {
                GPIO_InitTypeDef GPIO_InitStruct{0};
                // hardware pin 29 = PB6 = I2C2_SCL
                GPIO_InitStruct.Pin       = GPIO_PIN_6;
                GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
                GPIO_InitStruct.Pull      = GPIO_NOPULL;
                GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
                GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
                HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
                // hardware Pin 30 = PB7 = I2C1_SDA
                GPIO_InitStruct.Pin       = GPIO_PIN_7;
                GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
                GPIO_InitStruct.Pull      = GPIO_NOPULL;
                GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
                GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
                HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            } else {
                HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);
                HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
            }
            break;

        case gpio::group::spiSensor:
            if (enable) {
                // hardware pin 6 = A0 = CS-BME68X
                GPIO_InitTypeDef GPIO_InitStruct{0};
                GPIO_InitStruct.Pin   = GPIO_PIN_0;
                GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
                GPIO_InitStruct.Pull  = GPIO_NOPULL;
                GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            } else {
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);
            }
            enableDisableGpio(gpio::group::spi, enable);
            break;

        case gpio::group::spiMemory:
            if (enable) {
                GPIO_InitTypeDef GPIO_InitStruct{0};
                // hardware pin 8 = A2 = CS-SDcard
                GPIO_InitStruct.Pin   = GPIO_PIN_2;
                GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
                GPIO_InitStruct.Pull  = GPIO_NOPULL;
                GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
                // hardware pin 9 = A3 = SDcardDetect
                GPIO_InitStruct.Pin   = GPIO_PIN_3;
                GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
                GPIO_InitStruct.Pull  = GPIO_PULLUP;
                GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            } else {
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2);
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);
            }
            enableDisableGpio(gpio::group::spi, enable);
            break;

        case gpio::group::spi:
            if (enable) {
                GPIO_InitTypeDef GPIO_InitStruct{0};
                // hardware Pin 7 = A1 = SPI1_SCK
                GPIO_InitStruct.Pin       = GPIO_PIN_13;
                GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
                GPIO_InitStruct.Pull      = GPIO_NOPULL;
                GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;
                GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
                HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
                // hardware pin 12 = PA6 = SPI1 MISO
                GPIO_InitStruct.Pin       = GPIO_PIN_6;
                GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
                GPIO_InitStruct.Pull      = GPIO_NOPULL;
                GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;
                GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
                // hardware pin 13 = PA7 = SPI1_MOSI
                GPIO_InitStruct.Pin       = GPIO_PIN_10;
                GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
                GPIO_InitStruct.Pull      = GPIO_NOPULL;
                GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;
                GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            } else {
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_13);
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);
            }
            break;

        case gpio::group::debugPort:
            if (enable) {
                // These need to be set as after reset...
            } else {
                // NOTE : This resets the SWD AND JTAG pins to analog inputs (lowest power consumption).
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_13 | GPIO_PIN_14 );
                HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3 );
            }
            break;

        case gpio::group::uart1:
            if (enable) {
                GPIO_InitTypeDef GPIO_InitStruct{0};
                // hardwarepin 19 = PA9 = USART1_TX
                GPIO_InitStruct.Pin       = GPIO_PIN_9;
                GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
                GPIO_InitStruct.Pull      = GPIO_NOPULL;
                GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
                GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            } else {
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
            }
            break;

        default:
            // Error : unknown group
            break;
    }
#endif
}
