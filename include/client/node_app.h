#ifndef NODE_APP_H
#define NODE_APP_H

#include "stm32f0xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void node_app_init(UART_HandleTypeDef *huart);
void node_app_process(void);

#ifdef __cplusplus
}
#endif

#endif
