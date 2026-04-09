#ifndef CONTROLLER_APP_H
#define CONTROLLER_APP_H

#include "stm32f0xx_hal.h"

void controller_app_init(UART_HandleTypeDef *huart);
void controller_app_process(void);

#endif
