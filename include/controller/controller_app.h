#ifndef CONTROLLER_APP_H
#define CONTROLLER_APP_H

#include "stm32f0xx_hal.h"

void controller_app_init(UART_HandleTypeDef *huart);
void controller_app_process(void);
void controller_app_rx_complete_callback(UART_HandleTypeDef *huart);

#endif
