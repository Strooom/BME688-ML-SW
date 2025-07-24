// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <cstdint>

class gpio {
  public:
    gpio() = delete;
    enum class group : uint32_t {
        none = 0,
        spiSdCard,
        i2c1,
        i2c3,
        debugPort,
        uart1
    };

    static void enableClocks();
    static void enableGpio(group aGroup);
    static void disableGpio(group aGroup);
    static void disableAllGpio();

#ifndef unitTesting

  private:
#endif
    static void enableDisableGpio(group theGroup, bool enable);
};