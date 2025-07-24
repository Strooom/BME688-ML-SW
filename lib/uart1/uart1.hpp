// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#include <cstdint>

class uart1 {
  public:
    static void wakeUp();
    static void goSleep();
    static bool isInitialized() { return initalized; }

#ifndef unitTesting

  private:
#endif
    static bool initalized;
    static void initialize();
};
