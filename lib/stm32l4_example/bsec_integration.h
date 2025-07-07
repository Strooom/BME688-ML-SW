
/*!
 * @file bsec_integration.h
 *
 * @brief
 * Contains BSEC integration API
 */

/*!
 * @addtogroup bsec_examples BSEC Examples
 * @brief BSEC usage examples
 * @{*/

#ifndef __BSEC_INTEGRATION_H__
#define __BSEC_INTEGRATION_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**********************************************************************************************************************/
/* header files */
/**********************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bme68x.h"
#include "bsec_interface.h"
#include "bsec_datatypes.h"
#include "bsec_iot_example.h"


#define BSEC_CHECK_INPUT(x, shift)		(x & (1 << (shift-1)))
#define BSEC_TOTAL_HEAT_DUR             UINT16_C(140)
/**********************************************************************************************************************/
/* type definitions */
/**********************************************************************************************************************/

/* function pointer to the system specific sleep function */
typedef void (*sleep_fct)(uint32_t t_us,void *intf_ptr);

/* function pointer to the system specific timestamp derivation function */
typedef int64_t (*get_timestamp_us_fct)();

/* function pointer to the function processing obtained BSEC outputs */
typedef void (*output_ready_fct)(int64_t timestamp, float gas_estimate_1, float gas_estimate_2, float gas_estimate_3, float gas_estimate_4,
     float raw_pressure, float raw_temp, float raw_humidity, float raw_gas, uint8_t raw_gas_index, bsec_library_return_t bsec_status);

typedef void (*output_iaq_ready_fct)(int64_t timestamp, float iaq, uint8_t iaq_accuracy, float static_iaq, float co2_equivalent, float breath_voc_equivalent,
     float raw_pressure, float raw_temp, float raw_humidity, float raw_gas, uint8_t raw_gas_index, bsec_library_return_t bsec_status);


/* function pointer to the function loading a previous BSEC state from NVM */
typedef uint32_t (*state_load_fct)(uint8_t *state_buffer, uint32_t n_buffer);

/* function pointer to the function saving BSEC state to NVM */
typedef void (*state_save_fct)(const uint8_t *state_buffer, uint32_t length);

/* function pointer to the function loading the BSEC configuration string from NVM */
typedef uint32_t (*config_load_fct)(uint8_t *state_buffer, uint32_t n_buffer);
    
/* structure definitions */

/* Structure with the return value from bsec_iot_init() */
typedef struct{
	/*! Result of API execution status */
	int8_t bme68x_status;
	/*! Result of BSEC library */
	bsec_library_return_t bsec_status;
}return_values_init;
/**********************************************************************************************************************/
/* function declarations */
/**********************************************************************************************************************/

/*!
 * @brief       Initialize the bme68x sensor and the BSEC library
 *
 * @param[in]   sample_rate         mode to be used (either BSEC_SAMPLE_RATE_ULP or BSEC_SAMPLE_RATE_LP)
 * @param[in]   temperature_offset  device-specific temperature offset (due to self-heating)
 * @param[in]   bus_write           pointer to the bus writing function
 * @param[in]   bus_read            pointer to the bus reading function
 * @param[in]   sleep               pointer to the system-specific sleep function
 * @param[in]   state_load          pointer to the system-specific state load function
 *
 * @return      zero if successful, negative otherwise
 */
return_values_init bsec_iot_init(float sample_rate, float temperature_offset, bme68x_write_fptr_t bus_write, bme68x_read_fptr_t bus_read, 
    sleep_fct sleep_n, state_load_fct state_load, config_load_fct config_load, struct bme68x_dev dev);

/*!
 * @brief       Runs the main (endless) loop that queries sensor settings, applies them, and processes the measured data
 *
 * @param[in]   sleep               pointer to the system-specific sleep function
 * @param[in]   get_timestamp_us    pointer to the system-specific timestamp derivation function
 * @param[in]   output_ready        pointer to the function processing obtained BSEC outputs
 * @param[in]   state_save          pointer to the system-specific state save function
 * @param[in]   save_intvl          interval at which BSEC state should be saved (in samples)
 *
 * @return      return_values_init	struct with the result of the API and the BSEC library
 */ 
void bsec_iot_loop(sleep_fct sleep_n, get_timestamp_us_fct get_timestamp_us, output_ready_fct output_ready,
    state_save_fct state_save, uint32_t save_intvl);

#ifdef __cplusplus
}
#endif

#endif /* __BSEC_INTEGRATION_H__ */

/*! @}*/

