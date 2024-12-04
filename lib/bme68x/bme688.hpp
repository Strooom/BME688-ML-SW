// ######################################################################################
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once

#include <stdint.h>

class bme688 {
  public:
    explicit bme688(const uint8_t i2cAddress);
    bool isPresent();

#ifndef unitTesting

  private:
#endif
    enum class registers : uint8_t {
        chipId      = 0xD0,        // address of register holding chipId
        reset       = 0xE0,        // address of register to reset the device : Write 0xB6
        variant_id  = 0xF0,        // address of register to reset the device : Write 0xB6
        ctrl_hum    = 0x72,        // address of register to set humidity oversampling
        ctrl_meas   = 0x74,        // address of register to set temperature & pressure versampling
        config      = 0x75,        // address of register to set IIR filter & power mode
        temp_xlsb   = 0x24,        // address of register to read temperature XLSB
        temp_lsb    = 0x23,        // address of register to read temperature LSB
        temp_msb    = 0x22,        // address of register to read temperature MSB
        hum_lsb     = 0x26,        // address of register to read humidity LSB
        hum_msb     = 0x25,        // address of register to read humidity MSB
        press_xlsb  = 0x8C,        // address of register to read pressure LSB
        press_lsb   = 0x8B,        // address of register to read pressure LSB
        press_msb   = 0x8A,        // address of register to read pressure MSB
        meas_status = 0x1D,        // address of register to read measurement status
        status      = 0x73,        // address of register to read status
        meas_index  = 0x1E,        // address of register to read measurement index
        meas_period = 0x68,        // address of register to read measurement period
        gas_wait0   = 0x64,        // address of register to read gas wait time
        res_heat0   = 0x5A,        // address of register to read heater resistance
        idac_heat0  = 0x50,        // address of register to read heater current
        gas_r_lsb   = 0x2B,        // address of register to read gas resistance LSB
        gas_r_msb   = 0x2A,        // address of register to read gas resistance MSB
    };
    enum class commands : uint8_t {
        softReset = 0xB6,        // value to write to resetRegister to reset the BME68X
    };
    static constexpr uint8_t chipIdValue{0x61};        // value to expect at the chipIdregister, this allows to discover/recognize the BME68x
    static constexpr uint8_t halTrials{3};
    static constexpr uint32_t halTimeout{100};

    const uint8_t i2cAddress;
    bool testI2cAddress();
    uint8_t readRegister(registers registerToRead);
};
