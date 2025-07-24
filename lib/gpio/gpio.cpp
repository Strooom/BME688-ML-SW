#include <gpio.hpp>

#ifndef generic
#include "main.h"
#else

#endif

void gpio::enableClocks() {
#ifndef generic
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
#endif
}

void gpio::enableGpio(group aGroup) {
    enableDisableGpio(aGroup, true);
}

void gpio::disableGpio(group aGroup) {
    enableDisableGpio(aGroup, false);
}

void gpio::disableAllGpio() {
    enableDisableGpio(gpio::group::spiSdCard, false);
    enableDisableGpio(gpio::group::i2c1, false);
    enableDisableGpio(gpio::group::i2c3, false);
    enableDisableGpio(gpio::group::uart1, false);
    // NOTE : debugPort is not disabled as it is used for logging
}

void gpio::enableDisableGpio(group theGroup, bool enable) {
#ifndef generic
    enableClocks();

    switch (theGroup) {
        case gpio::group::spiSdCard:
            if (enable) {
                GPIO_InitTypeDef GpioSettings{0};
                // hardware pin 8 = PA2 = CS-SDcard
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
                GpioSettings.Pin   = GPIO_PIN_2;
                GpioSettings.Mode  = GPIO_MODE_OUTPUT_PP;
                GpioSettings.Pull  = GPIO_NOPULL;        // CS has a pullup resistor on the PCB
                GpioSettings.Speed = GPIO_SPEED_FREQ_LOW;
                HAL_GPIO_Init(GPIOA, &GpioSettings);
                // hardware pin 9 = PA3 = SDcardDetect
                GpioSettings.Pin   = GPIO_PIN_3;
                GpioSettings.Mode  = GPIO_MODE_INPUT;
                GpioSettings.Pull  = GPIO_PULLUP;        // There is no pullup resistor on the PCB
                GpioSettings.Speed = GPIO_SPEED_FREQ_LOW;
                HAL_GPIO_Init(GPIOA, &GpioSettings);
                // hardware pin 7 = PA1 = SPI1_SCK
                GpioSettings.Pin       = GPIO_PIN_1;
                GpioSettings.Mode      = GPIO_MODE_AF_PP;
                GpioSettings.Pull      = GPIO_NOPULL;
                GpioSettings.Speed     = GPIO_SPEED_FREQ_MEDIUM;
                GpioSettings.Alternate = GPIO_AF5_SPI1;
                HAL_GPIO_Init(GPIOA, &GpioSettings);
                // hardware pin 21 = PA11 = SPI1 MISO
                GpioSettings.Pin       = GPIO_PIN_11;
                GpioSettings.Mode      = GPIO_MODE_AF_PP;
                GpioSettings.Pull      = GPIO_NOPULL;
                GpioSettings.Speed     = GPIO_SPEED_FREQ_MEDIUM;
                GpioSettings.Alternate = GPIO_AF5_SPI1;
                HAL_GPIO_Init(GPIOA, &GpioSettings);
                // hardware pin 22 = PA12= SPI1_MOSI
                GpioSettings.Pin       = GPIO_PIN_12;
                GpioSettings.Mode      = GPIO_MODE_AF_PP;
                GpioSettings.Pull      = GPIO_NOPULL;
                GpioSettings.Speed     = GPIO_SPEED_FREQ_MEDIUM;
                GpioSettings.Alternate = GPIO_AF5_SPI1;
                HAL_GPIO_Init(GPIOA, &GpioSettings);
            } else {
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2);
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1);
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11);
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);
            }
            break;

        case gpio::group::i2c1:
            if (enable) {
                GPIO_InitTypeDef GpioSettings{0};
                // hardware pin 19 = PA9 = I2C1_SCL
                GpioSettings.Pin       = GPIO_PIN_9;
                GpioSettings.Mode      = GPIO_MODE_AF_OD;
                GpioSettings.Pull      = GPIO_NOPULL;
                GpioSettings.Speed     = GPIO_SPEED_FREQ_LOW;
                GpioSettings.Alternate = GPIO_AF4_I2C1;
                HAL_GPIO_Init(GPIOA, &GpioSettings);
                // hardware Pin 20 = PA10 = I2C1_SDA
                GpioSettings.Pin       = GPIO_PIN_10;
                GpioSettings.Mode      = GPIO_MODE_AF_OD;
                GpioSettings.Pull      = GPIO_NOPULL;
                GpioSettings.Speed     = GPIO_SPEED_FREQ_LOW;
                GpioSettings.Alternate = GPIO_AF4_I2C1;
                HAL_GPIO_Init(GPIOA, &GpioSettings);
            } else {
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);
            }
            break;

        case gpio::group::i2c3:
            if (enable) {
                GPIO_InitTypeDef GpioSettings{0};
                // hardware pin 13 = PA7 = I2C3_SCL
                GpioSettings.Pin       = GPIO_PIN_7;
                GpioSettings.Mode      = GPIO_MODE_AF_OD;
                GpioSettings.Pull      = GPIO_NOPULL;
                GpioSettings.Speed     = GPIO_SPEED_FREQ_LOW;
                GpioSettings.Alternate = GPIO_AF4_I2C1;
                HAL_GPIO_Init(GPIOA, &GpioSettings);
                // hardware Pin 27 = PB4 = I2C3_SDA
                GpioSettings.Pin       = GPIO_PIN_4;
                GpioSettings.Mode      = GPIO_MODE_AF_OD;
                GpioSettings.Pull      = GPIO_NOPULL;
                GpioSettings.Speed     = GPIO_SPEED_FREQ_LOW;
                GpioSettings.Alternate = GPIO_AF4_I2C1;
                HAL_GPIO_Init(GPIOB, &GpioSettings);
            } else {
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);
                HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4);
            }
            break;

        case gpio::group::debugPort:
            if (enable) {
                // These need to be set as after reset...
            } else {
                // NOTE : This resets the SWD AND JTAG pins to analog inputs (lowest power consumption).
                HAL_GPIO_DeInit(GPIOA, GPIO_PIN_13 | GPIO_PIN_14);
                HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3);
            }
            break;

        case gpio::group::uart1:
            if (enable) {
                GPIO_InitTypeDef GpioSettings{0};
                // hardware pin 29 = PB6 = USART1_TX
                GpioSettings.Pin       = GPIO_PIN_6;
                GpioSettings.Mode      = GPIO_MODE_AF_PP;
                GpioSettings.Pull      = GPIO_NOPULL;
                GpioSettings.Speed     = GPIO_SPEED_FREQ_LOW;
                GpioSettings.Alternate = GPIO_AF7_USART1;
                HAL_GPIO_Init(GPIOB, &GpioSettings);
            } else {
                HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);
            }
            break;

        default:
            // Error : unknown group
            break;
    }
#endif
}
