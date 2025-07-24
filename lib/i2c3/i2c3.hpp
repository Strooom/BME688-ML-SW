// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include <cstdint>
#include <bme68x_defs.h>        // needed as return type for board/MCU-specific functions for reading and writing to the BME68x sensor

class i2c3 {
  public:
    static void wakeUp();
    static void goSleep();
    static bool isInitialized() { return initalized; }  
    static BME68X_INTF_RET_TYPE read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
    static BME68X_INTF_RET_TYPE write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);

#ifndef unitTesting

  private:
#endif

    static bool initalized;
    static void initialize();
};
