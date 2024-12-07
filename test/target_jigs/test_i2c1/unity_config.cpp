#include "unity_config.h"
#include "main.h"
#include <uart1.hpp>

#ifdef TARGET_TEST_PORT_UART1
extern UART_HandleTypeDef huart1;
void unityOutputStart() {
    uart1::initialize();
}
void unityOutputChar(char c) {
    HAL_UART_Transmit(&huart1, (uint8_t *)(&c), 1, 1000);
}
#endif

void unityOutputFlush() {}

void unityOutputComplete() {}