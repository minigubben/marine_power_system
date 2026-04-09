#ifndef CLIENT_NODE_CONFIG_H
#define CLIENT_NODE_CONFIG_H

#include "stm32f0xx_hal.h"

#define CLIENT_ROLE_INPUT 1U
#define CLIENT_ROLE_OUTPUT 2U

#ifndef CLIENT_NODE_ID
#define CLIENT_NODE_ID 1U
#endif

#ifndef CLIENT_ROLE
#define CLIENT_ROLE CLIENT_ROLE_INPUT
#endif

#ifndef BUTTON_1_ID
#define BUTTON_1_ID 1U
#endif

#ifndef BUTTON_1_GPIO_Port
#define BUTTON_1_GPIO_Port GPIOA
#endif

#ifndef BUTTON_1_Pin
#define BUTTON_1_Pin GPIO_PIN_0
#endif

#ifndef BUTTON_1_PULL_MODE
#define BUTTON_1_PULL_MODE GPIO_PULLUP
#endif

#ifndef BUTTON_1_ACTIVE_STATE
#define BUTTON_1_ACTIVE_STATE GPIO_PIN_RESET
#endif

#ifndef BUTTON_DEBOUNCE_MS
#define BUTTON_DEBOUNCE_MS 40U
#endif

#endif
