#ifndef NODE_CONFIG_H
#define NODE_CONFIG_H

#include "main.h"

#ifndef NODE_ID
#define NODE_ID 10U
#endif

#ifndef NODE_HAS_INPUTS
#define NODE_HAS_INPUTS 0U
#endif

#ifndef NODE_HAS_OUTPUTS
#define NODE_HAS_OUTPUTS 1U
#endif

#ifndef NODE_BUTTON_1_ID
#define NODE_BUTTON_1_ID 1U
#endif

#ifndef NODE_BUTTON_1_GPIO_Port
#define NODE_BUTTON_1_GPIO_Port GPIOA
#endif

#ifndef NODE_BUTTON_1_Pin
#define NODE_BUTTON_1_Pin GPIO_PIN_0
#endif

#ifndef NODE_BUTTON_1_PULL_MODE
#define NODE_BUTTON_1_PULL_MODE GPIO_PULLUP
#endif

#ifndef NODE_BUTTON_1_ACTIVE_STATE
#define NODE_BUTTON_1_ACTIVE_STATE GPIO_PIN_RESET
#endif

#ifndef NODE_BUTTON_DEBOUNCE_MS
#define NODE_BUTTON_DEBOUNCE_MS 40U
#endif

#ifndef NODE_OUTPUT_1_ID
#define NODE_OUTPUT_1_ID 1U
#endif

#ifndef NODE_OUTPUT_1_PORT
#define NODE_OUTPUT_1_PORT Output_1_GPIO_Port
#endif

#ifndef NODE_OUTPUT_1_PIN
#define NODE_OUTPUT_1_PIN Output_1_Pin
#endif

#ifndef NODE_OUTPUT_2_ID
#define NODE_OUTPUT_2_ID 2U
#endif

#ifndef NODE_OUTPUT_2_PORT
#define NODE_OUTPUT_2_PORT Output_2_GPIO_Port
#endif

#ifndef NODE_OUTPUT_2_PIN
#define NODE_OUTPUT_2_PIN Output_2_Pin
#endif

#ifndef NODE_OUTPUT_3_ID
#define NODE_OUTPUT_3_ID 3U
#endif

#ifndef NODE_OUTPUT_3_PORT
#define NODE_OUTPUT_3_PORT Output_3_GPIO_Port
#endif

#ifndef NODE_OUTPUT_3_PIN
#define NODE_OUTPUT_3_PIN Output_3_Pin
#endif

#endif
