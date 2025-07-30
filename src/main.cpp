#include <main.h>
#include <clock.hpp>
#include <fatfs.h>
#include <cstdio>
#include <cstring>
#include <gpio.hpp>
#include <uart1.hpp>
#include <i2c1.hpp>
#include <i2c3.hpp>
#include <spi.hpp>
#include <delay.hpp>
#include <logging.hpp>
#include <float.hpp>
#include <version.hpp>

#include <bme68x_defs.h>
#include <bme68x.h>
#include <bsec_interface.h>
// #include <bsec_selectivity.h>
#include "detect vinegar_354_510.h"
#include <stm32l4xx_hal_msp.c>
#include <stm32l4xx_it.c>

#include <bsec2.h>

static void MX_GPIO_Init(void);
static void MX_LPTIM1_Init(void);

#define NUM_OF_SENS 2
#define SAMPLE_RATE BSEC_SAMPLE_RATE_SCAN

Bsec2 envSensor[NUM_OF_SENS];
uint8_t bsecMemBlock[NUM_OF_SENS][BSEC_INSTANCE_SIZE];
uint8_t sensor = 0;

void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec) {
    if (!outputs.nOutputs) {
        return;
    }

uint32_t minutes{0};
uint32_t seconds{0};
uint32_t milliseconds{0};

minutes = static_cast<uint32_t>(outputs.output[0].time_stamp / INT64_C(60000000000));
seconds = static_cast<uint32_t>((outputs.output[0].time_stamp % INT64_C(60000000000)) / INT64_C(1000000000));
milliseconds = static_cast<uint32_t>((outputs.output[0].time_stamp % INT64_C(1000000000)) / INT64_C(1000000));

    logging::snprintf("\n%02d:%02d:%03d : sensor[%d]\n", minutes, seconds, milliseconds, sensor);
    for (uint8_t i = 0; i < outputs.nOutputs; i++) {
        const bsecData output = outputs.output[i];
        switch (output.sensor_id) {
            case BSEC_OUTPUT_IAQ:
                logging::snprintf("IAQ = %d \n", (int)output.signal);
                logging::snprintf("IAQ accuracy = %d \n", (int)output.accuracy);
                break;
            case BSEC_OUTPUT_RAW_TEMPERATURE:
                logging::snprintf("Temperature = %d.%02d\n", integerPart(output.signal, 2), fractionalPart(output.signal, 2));
                break;
            case BSEC_OUTPUT_RAW_PRESSURE:
                logging::snprintf("Pressure = %d \n", (int)output.signal);
                break;
            case BSEC_OUTPUT_RAW_HUMIDITY:
                logging::snprintf("Humidity = %d \n", (int)output.signal);
                break;
            case BSEC_OUTPUT_RAW_GAS:
                logging::snprintf("Gas resistance = %d \n", (int)output.signal);
                break;
            case BSEC_OUTPUT_STABILIZATION_STATUS:
                logging::snprintf("Stabilization status = %d \n", (int)output.signal);
                break;
            case BSEC_OUTPUT_RUN_IN_STATUS:
                logging::snprintf("Run in status = %d \n", (int)output.signal);
                break;
            case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE:
                logging::snprintf("Compensated temperature = %d \n", (int)output.signal);
                break;
            case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY:
                logging::snprintf("Compensated humidity = %d \n", (int)output.signal);
                break;
            case BSEC_OUTPUT_STATIC_IAQ:
                logging::snprintf("Static IAQ = %d \n", (int)output.signal);
                break;
            case BSEC_OUTPUT_CO2_EQUIVALENT:
                logging::snprintf("CO2 Equivalent = %d \n", (int)output.signal);
                break;
            case BSEC_OUTPUT_BREATH_VOC_EQUIVALENT:
                logging::snprintf("bVOC equivalent = %d \n", (int)output.signal);
                break;
            case BSEC_OUTPUT_GAS_PERCENTAGE:
                logging::snprintf("Gas percentage = %d \n", (int)output.signal);
                break;
            case BSEC_OUTPUT_COMPENSATED_GAS:
                logging::snprintf("Compensated gas = %d \n", (int)output.signal);
                break;
            case BSEC_OUTPUT_GAS_ESTIMATE_1:
                logging::snprintf("Pure air : %d.%02d\n", integerPart(output.signal, 2), fractionalPart(output.signal, 2));
                break;
            case BSEC_OUTPUT_GAS_ESTIMATE_2:
                logging::snprintf("Vinegar  : %d.%02d\n", integerPart(output.signal, 2), fractionalPart(output.signal, 2));
                break;
            case BSEC_OUTPUT_RAW_GAS_INDEX:
                logging::snprintf("Raw gas index = %d \n", (int)output.signal);
                break;
            default:
                logging::snprintf("other output %d \n", (int)(output.signal * 100.0F));
                break;
        }
    }
}

void checkBsecStatus(Bsec2 bsec) {
    if (bsec.status < BSEC_OK) {
        logging::snprintf("BSEC error code %d \n", bsec.status);
    } else if (bsec.status > BSEC_OK) {
        logging::snprintf("BSEC warning code : %d \n", bsec.status);
    }
    if (bsec.sensor.status < BME68X_OK) {
        logging::snprintf("BME68X error code : %d \n", bsec.sensor.status);
    } else if (bsec.sensor.status > BME68X_OK) {
        logging::snprintf("BME68X warning code : %d \n", bsec.sensor.status);
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();

    HAL_Delay(1000);

//    MX_GPIO_Init();
//    MX_LPTIM1_Init();
//    MX_FATFS_Init();

    gpio::enableClocks();
    uart1::wakeUp();
    logging::enable(logging::destination::uart1);
    i2c3::wakeUp();

    /* Desired subscription list of BSEC2 outputs */
    bsecSensor sensorList[] = {
        // BSEC_OUTPUT_IAQ,
        // BSEC_OUTPUT_RAW_TEMPERATURE,
        // BSEC_OUTPUT_RAW_PRESSURE,
        // BSEC_OUTPUT_RAW_HUMIDITY,
        BSEC_OUTPUT_GAS_ESTIMATE_1,
        BSEC_OUTPUT_GAS_ESTIMATE_2,
        BSEC_OUTPUT_RAW_GAS,
        BSEC_OUTPUT_RAW_GAS_INDEX,
        // BSEC_OUTPUT_STABILIZATION_STATUS,
        // BSEC_OUTPUT_RUN_IN_STATUS,
        // BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
        // BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
        // BSEC_OUTPUT_STATIC_IAQ,
        // BSEC_OUTPUT_CO2_EQUIVALENT,
        // BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
        // BSEC_OUTPUT_GAS_PERCENTAGE,
        // BSEC_OUTPUT_COMPENSATED_GAS
    };

    uint8_t sensorI2cAddress[2]{BME68X_I2C_ADDR_LOW, BME68X_I2C_ADDR_HIGH};

    for (uint8_t i = 0; i < NUM_OF_SENS; i++) {
        /* Assigning a chunk of memory block to the bsecInstance */
        envSensor[i].allocateMemory(bsecMemBlock[i]);

        /* Initialize the library and interfaces */

        if (!envSensor[i].begin(BME68X_I2C_INTF, i2c3::read, i2c3::write, delay_us, &sensorI2cAddress[i])) {
            checkBsecStatus(envSensor[i]);
        }

        if (!envSensor[i].setConfig(bsec_config_selectivity)) {
            checkBsecStatus(envSensor[i]);
        }

        /* Subscribe to the desired BSEC2 outputs */
        if (!envSensor[i].updateSubscription(sensorList, ARRAY_LEN(sensorList), SAMPLE_RATE)) {
            checkBsecStatus(envSensor[i]);
        }

        /* Whenever new data is available call the newDataCallback function */
        envSensor[i].attachCallback(newDataCallback);
    }

    // Serial.println("BSEC library version " + String(envSensor[0].version.major) + "." + String(envSensor[0].version.minor) + "." + String(envSensor[0].version.major_bugfix) + "." + String(envSensor[0].version.minor_bugfix));
    logging::snprintf("\n\n\nBSEC Initialized\n");

    while (true) {
        /* Call the run function often so that the library can check if it is time to read new data from the sensor and process it. */
        for (sensor = 0; sensor < NUM_OF_SENS; sensor++) {
            if (!envSensor[sensor].run()) {
                checkBsecStatus(envSensor[sensor]);
            }
        }
    }
}

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
    while (true) {
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
