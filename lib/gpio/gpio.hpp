// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>

class gpio {
  public:
    gpio() = delete;
    enum class group : uint32_t {
        none = 0,
        spiSensor,
        spiMemory,
        spi,
        i2c,
        debugPort,
        uart1
    };

    static void enableGpio(group aGroup);
    static void disableGpio(group aGroup);
    static void disableAllGpio();

#ifndef unitTesting

  private:
#endif
    static void enableDisableGpio(group theGroup, bool enable);
};