#include <main.h>
#include <clock.hpp>
#include <fatfs.h>
#include <stdio.h>
#include <string.h>
#include <gpio.hpp>
#include <uart1.hpp>
#include <i2c3.hpp>
#include <delay.hpp>
#include <logging.hpp>
#include <version.hpp>

#include <bme68x_defs.h>
#include <bme68x.h>
#include <stm32l4xx_hal_msp.c>
#include <stm32l4xx_it.c>


I2C_HandleTypeDef hi2c1;            // I2C1 = communication with mother board = MuMo
I2C_HandleTypeDef hi2c3;            // I2C3 = communication with BME688 sensors
LPTIM_HandleTypeDef hlptim1;        // low power internal timing
SPI_HandleTypeDef hspi1;            // SPI = towards SD card
UART_HandleTypeDef huart1;          // UART1 = communication with debug console

// void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_LPTIM1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);


int main(void) {
    HAL_Init();
    SystemClock_Config();

    // HAL_Delay(8000);        // give the debugger time to connect

    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_LPTIM1_Init();
    MX_SPI1_Init();
    MX_FATFS_Init();
    

    gpio::enableClocks();
    uart1::wakeUp();
    logging::enable(logging::destination::uart1);
    i2c3::wakeUp();

    struct bme68x_dev sensor0;
    memset(&sensor0, 0, sizeof(sensor0));
    uint8_t sensor0I2cAddress = BME68X_I2C_ADDR_LOW;
    sensor0.intf              = BME68X_I2C_INTF;
    sensor0.read              = i2c3::read;
    sensor0.write             = i2c3::write;
    sensor0.delay_us          = delay_us;
    sensor0.intf_ptr          = &sensor0I2cAddress;
    sensor0.amb_temp          = 22;

    struct bme68x_dev sensor1;
    memset(&sensor1, 0, sizeof(sensor1));
    uint8_t sensor1I2cAddress = BME68X_I2C_ADDR_HIGH;
    sensor1.intf              = BME68X_I2C_INTF;
    sensor1.read              = i2c3::read;
    sensor1.write             = i2c3::write;
    sensor1.delay_us          = delay_us;
    sensor1.intf_ptr          = &sensor1I2cAddress;
    sensor1.amb_temp          = 22;

    int8_t initResult;
    initResult = bme68x_init(&sensor0);
    logging::snprintf("BME68X chip ID=0x%02x\n", sensor0.chip_id);
    initResult = bme68x_init(&sensor1);
    logging::snprintf("BME68X chip ID=0x%02x\n", sensor1.chip_id);

    while (true) {
    }
}


/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {
    /* USER CODE BEGIN I2C1_Init 0 */

    /* USER CODE END I2C1_Init 0 */

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
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

    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C1_Init 2 */

    /* USER CODE END I2C1_Init 2 */
}

/**
 * @brief I2C3 Initialization Function
 * @param None
 * @retval None
 */
// static void MX_I2C3_Init(void) {
//     /* USER CODE BEGIN I2C3_Init 0 */

//     /* USER CODE END I2C3_Init 0 */

//     /* USER CODE BEGIN I2C3_Init 1 */

//     /* USER CODE END I2C3_Init 1 */
//     hi2c3.Instance              = I2C3;
//     hi2c3.Init.Timing           = 0x00B07CB4;
//     hi2c3.Init.OwnAddress1      = 0;
//     hi2c3.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
//     hi2c3.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
//     hi2c3.Init.OwnAddress2      = 0;
//     hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//     hi2c3.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
//     hi2c3.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
//     if (HAL_I2C_Init(&hi2c3) != HAL_OK) {
//         Error_Handler();
//     }

//     /** Configure Analogue filter
//      */
//     if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
//         Error_Handler();
//     }

//     /** Configure Digital filter
//      */
//     if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK) {
//         Error_Handler();
//     }
//     /* USER CODE BEGIN I2C3_Init 2 */

//     /* USER CODE END I2C3_Init 2 */
// }

/**
 * @brief LPTIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_LPTIM1_Init(void) {
    /* USER CODE BEGIN LPTIM1_Init 0 */

    /* USER CODE END LPTIM1_Init 0 */

    /* USER CODE BEGIN LPTIM1_Init 1 */

    /* USER CODE END LPTIM1_Init 1 */
    hlptim1.Instance             = LPTIM1;
    hlptim1.Init.Clock.Source    = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
    hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
    hlptim1.Init.Trigger.Source  = LPTIM_TRIGSOURCE_SOFTWARE;
    hlptim1.Init.OutputPolarity  = LPTIM_OUTPUTPOLARITY_HIGH;
    hlptim1.Init.UpdateMode      = LPTIM_UPDATE_IMMEDIATE;
    hlptim1.Init.CounterSource   = LPTIM_COUNTERSOURCE_INTERNAL;
    hlptim1.Init.Input1Source    = LPTIM_INPUT1SOURCE_GPIO;
    hlptim1.Init.Input2Source    = LPTIM_INPUT2SOURCE_GPIO;
    if (HAL_LPTIM_Init(&hlptim1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN LPTIM1_Init 2 */

    /* USER CODE END LPTIM1_Init 2 */
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {
    /* USER CODE BEGIN SPI1_Init 0 */

    /* USER CODE END SPI1_Init 0 */

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    hspi1.Instance               = SPI1;
    hspi1.Init.Mode              = SPI_MODE_MASTER;
    hspi1.Init.Direction         = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize          = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity       = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase          = SPI_PHASE_1EDGE;
    hspi1.Init.NSS               = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
    hspi1.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode            = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial     = 7;
    hspi1.Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE;
    hspi1.Init.NSSPMode          = SPI_NSS_PULSE_ENABLE;
    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
// static void MX_USART1_UART_Init(void) {
//     /* USER CODE BEGIN USART1_Init 0 */

//     /* USER CODE END USART1_Init 0 */

//     /* USER CODE BEGIN USART1_Init 1 */

//     /* USER CODE END USART1_Init 1 */
//     huart1.Instance                    = USART1;
//     huart1.Init.BaudRate               = 115200;
//     huart1.Init.WordLength             = UART_WORDLENGTH_8B;
//     huart1.Init.StopBits               = UART_STOPBITS_1;
//     huart1.Init.Parity                 = UART_PARITY_NONE;
//     huart1.Init.Mode                   = UART_MODE_TX_RX;
//     huart1.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
//     huart1.Init.OverSampling           = UART_OVERSAMPLING_16;
//     huart1.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
//     huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//     if (HAL_UART_Init(&huart1) != HAL_OK) {
//         Error_Handler();
//     }
//     /* USER CODE BEGIN USART1_Init 2 */

//     /* USER CODE END USART1_Init 2 */
// }

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* USER CODE BEGIN MX_GPIO_Init_1 */
    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(CS_SDCARD_GPIO_Port, CS_SDCARD_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : CS_SDCARD_Pin */
    GPIO_InitStruct.Pin   = CS_SDCARD_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CS_SDCARD_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : cardDetect_Pin */
    GPIO_InitStruct.Pin  = cardDetect_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(cardDetect_GPIO_Port, &GPIO_InitStruct);

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

int fputc(int c, FILE *stream) {
    HAL_UART_Transmit(&huart1, (unsigned char *)&c, 1, 1000);
    return 1;
}
