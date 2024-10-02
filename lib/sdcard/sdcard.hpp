#pragma once

class sdCard {
  public:
    static bool isPresent() {
        return (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET);
    }
};