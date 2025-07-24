// ######################################################################################
// ### Author : Pascal Roobrouck  https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA  https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <logging.hpp>
#include <gpio.hpp>
#ifndef generic
#include "main.h"
extern UART_HandleTypeDef huart1;
#endif

uint32_t logging::activeSources{0};
uint32_t logging::activeDestinations{0};
char logging::buffer[bufferLength]{};

void logging::initialize() {
#ifndef generic
    if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) == 0x0001) {        // is a SWD debugprobe connected ?
#ifndef platformio                                                             // SWO TRACE is not working on PlatformIO
        logging::enable(logging::destination::debugProbe);
#endif
    }
#endif
    if (!logging::isActive(logging::destination::debugProbe)) {
        gpio::disableGpio(gpio::group::debugPort);
    }
}

uint32_t logging::snprintf(const char *format, ...) {
    uint32_t length{0};
    if (activeDestinations != 00) {
        va_list argList;
        va_start(argList, format);
        length = vsnprintf(buffer, bufferLength, format, argList);
        va_end(argList);
        write(length);
    }
    return length;
}

uint32_t logging::snprintf(source aSource, const char *format, ...) {
    uint32_t length{0};
    if (activeDestinations != 00) {
        if (isActive(aSource)) {
            va_list argList;
            va_start(argList, format);
            length = vsnprintf(buffer, bufferLength, format, argList);
            va_end(argList);
            write(length);
        }
    }
    return length;
}

void logging::write(uint32_t dataLength) {
    if (isActive(destination::debugProbe)) {
        for (uint32_t index = 0; index < dataLength; index++) {
#ifndef generic
            ITM_SendChar(static_cast<uint32_t>(buffer[index]));
#endif
        }
    }
    if (isActive(destination::uart1)) {
#ifndef generic
        HAL_UART_Transmit(&huart1, (uint8_t *)buffer, static_cast<const uint16_t>(dataLength), 1000);
#endif
    }
}

void logging::dump() {
    logging::snprintf("logging : \n");
    if (activeSources == 0) {
        logging::snprintf("  no active sources\n");
    } else {
        logging::snprintf("  sources      : ");
        for (uint32_t index = 0; index < 32; index++) {
            if ((activeSources & (0x01 << index)) != 0) {
                logging::snprintf("%s ", toString(static_cast<source>(index)));
            }
        }
        logging::snprintf("\n");
    }
    logging::snprintf("  destinations : ");
    for (uint32_t index = 0; index < 32; index++) {
        if ((activeDestinations & (0x01 << index)) != 0) {
            logging::snprintf("%s ", toString(static_cast<destination>(index)));
        }
    }
    logging::snprintf("\n");
}
const char *toString(logging::source aSource) {
    switch (aSource) {
        case logging::source::applicationEvents:
            return "applicationEvents";
        case logging::source::sensorEvents:
            return "sensorEvents";
        case logging::source::sensorData:
            return "sensorData";
        case logging::source::sdCardData:
            return "sdCardData";
        case logging::source::sdCardEvents:
            return "sdCardEvents";
        case logging::source::warning:
            return "warning";
        case logging::source::error:
            return "error";
        case logging::source::criticalError:
            return "criticalError";
        default:
            return "unknown";
    }
}

const char *toString(logging::destination aDestination) {
    switch (aDestination) {
        case logging::destination::debugProbe:
            return "debugProbe";
        case logging::destination::uart1:
            return "uart1";
        default:
            return "unknown";
    }
}
