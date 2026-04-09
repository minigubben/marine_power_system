#ifndef NODE_CONFIG_H
#define NODE_CONFIG_H

#include "main.h"

#ifndef NODE_ID
#define NODE_ID 10U
#endif

#if defined(NODE_BOARD_INPUT_PANEL)

#ifndef NODE_HAS_INPUTS
#define NODE_HAS_INPUTS 1U
#endif

#ifndef NODE_HAS_OUTPUTS
#define NODE_HAS_OUTPUTS 1U
#endif

#ifndef NODE_INPUT_COUNT
#define NODE_INPUT_COUNT 6U
#endif

#ifndef NODE_OUTPUT_COUNT
#define NODE_OUTPUT_COUNT 6U
#endif

#ifndef NODE_BUTTON_1_ID
#define NODE_BUTTON_1_ID 1U
#endif

#ifndef NODE_BUTTON_1_GPIO_Port
#define NODE_BUTTON_1_GPIO_Port Input_1_GPIO_Port
#endif

#ifndef NODE_BUTTON_1_Pin
#define NODE_BUTTON_1_Pin Input_1_Pin
#endif

#ifndef NODE_BUTTON_1_PULL_MODE
#define NODE_BUTTON_1_PULL_MODE GPIO_PULLUP
#endif

#ifndef NODE_BUTTON_1_ACTIVE_STATE
#define NODE_BUTTON_1_ACTIVE_STATE GPIO_PIN_RESET
#endif

#ifndef NODE_BUTTON_2_ID
#define NODE_BUTTON_2_ID 2U
#endif

#ifndef NODE_BUTTON_2_GPIO_Port
#define NODE_BUTTON_2_GPIO_Port Input_2_GPIO_Port
#endif

#ifndef NODE_BUTTON_2_Pin
#define NODE_BUTTON_2_Pin Input_2_Pin
#endif

#ifndef NODE_BUTTON_2_PULL_MODE
#define NODE_BUTTON_2_PULL_MODE GPIO_PULLUP
#endif

#ifndef NODE_BUTTON_2_ACTIVE_STATE
#define NODE_BUTTON_2_ACTIVE_STATE GPIO_PIN_RESET
#endif

#ifndef NODE_BUTTON_3_ID
#define NODE_BUTTON_3_ID 3U
#endif

#ifndef NODE_BUTTON_3_GPIO_Port
#define NODE_BUTTON_3_GPIO_Port Input_3_GPIO_Port
#endif

#ifndef NODE_BUTTON_3_Pin
#define NODE_BUTTON_3_Pin Input_3_Pin
#endif

#ifndef NODE_BUTTON_3_PULL_MODE
#define NODE_BUTTON_3_PULL_MODE GPIO_PULLUP
#endif

#ifndef NODE_BUTTON_3_ACTIVE_STATE
#define NODE_BUTTON_3_ACTIVE_STATE GPIO_PIN_RESET
#endif

#ifndef NODE_BUTTON_4_ID
#define NODE_BUTTON_4_ID 4U
#endif

#ifndef NODE_BUTTON_4_GPIO_Port
#define NODE_BUTTON_4_GPIO_Port Input_4_GPIO_Port
#endif

#ifndef NODE_BUTTON_4_Pin
#define NODE_BUTTON_4_Pin Input_4_Pin
#endif

#ifndef NODE_BUTTON_4_PULL_MODE
#define NODE_BUTTON_4_PULL_MODE GPIO_PULLUP
#endif

#ifndef NODE_BUTTON_4_ACTIVE_STATE
#define NODE_BUTTON_4_ACTIVE_STATE GPIO_PIN_RESET
#endif

#ifndef NODE_BUTTON_5_ID
#define NODE_BUTTON_5_ID 5U
#endif

#ifndef NODE_BUTTON_5_GPIO_Port
#define NODE_BUTTON_5_GPIO_Port Input_5_GPIO_Port
#endif

#ifndef NODE_BUTTON_5_Pin
#define NODE_BUTTON_5_Pin Input_5_Pin
#endif

#ifndef NODE_BUTTON_5_PULL_MODE
#define NODE_BUTTON_5_PULL_MODE GPIO_PULLUP
#endif

#ifndef NODE_BUTTON_5_ACTIVE_STATE
#define NODE_BUTTON_5_ACTIVE_STATE GPIO_PIN_RESET
#endif

#ifndef NODE_BUTTON_6_ID
#define NODE_BUTTON_6_ID 6U
#endif

#ifndef NODE_BUTTON_6_GPIO_Port
#define NODE_BUTTON_6_GPIO_Port Input_6_GPIO_Port
#endif

#ifndef NODE_BUTTON_6_Pin
#define NODE_BUTTON_6_Pin Input_6_Pin
#endif

#ifndef NODE_BUTTON_6_PULL_MODE
#define NODE_BUTTON_6_PULL_MODE GPIO_PULLUP
#endif

#ifndef NODE_BUTTON_6_ACTIVE_STATE
#define NODE_BUTTON_6_ACTIVE_STATE GPIO_PIN_RESET
#endif

#elif defined(NODE_BOARD_OUTPUT_8CH)

#ifndef NODE_HAS_INPUTS
#define NODE_HAS_INPUTS 0U
#endif

#ifndef NODE_HAS_OUTPUTS
#define NODE_HAS_OUTPUTS 1U
#endif

#ifndef NODE_INPUT_COUNT
#define NODE_INPUT_COUNT 0U
#endif

#ifndef NODE_OUTPUT_COUNT
#define NODE_OUTPUT_COUNT 8U
#endif

#else
#error "Unsupported node board profile"
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

#ifndef NODE_OUTPUT_4_ID
#define NODE_OUTPUT_4_ID 4U
#endif

#ifndef NODE_OUTPUT_4_PORT
#define NODE_OUTPUT_4_PORT Output_4_GPIO_Port
#endif

#ifndef NODE_OUTPUT_4_PIN
#define NODE_OUTPUT_4_PIN Output_4_Pin
#endif

#ifndef NODE_OUTPUT_5_ID
#define NODE_OUTPUT_5_ID 5U
#endif

#ifndef NODE_OUTPUT_5_PORT
#define NODE_OUTPUT_5_PORT Output_5_GPIO_Port
#endif

#ifndef NODE_OUTPUT_5_PIN
#define NODE_OUTPUT_5_PIN Output_5_Pin
#endif

#ifndef NODE_OUTPUT_6_ID
#define NODE_OUTPUT_6_ID 6U
#endif

#ifndef NODE_OUTPUT_6_PORT
#define NODE_OUTPUT_6_PORT Output_6_GPIO_Port
#endif

#ifndef NODE_OUTPUT_6_PIN
#define NODE_OUTPUT_6_PIN Output_6_Pin
#endif

#if NODE_OUTPUT_COUNT >= 7U
#ifndef NODE_OUTPUT_7_ID
#define NODE_OUTPUT_7_ID 7U
#endif

#ifndef NODE_OUTPUT_7_PORT
#define NODE_OUTPUT_7_PORT Output_7_GPIO_Port
#endif

#ifndef NODE_OUTPUT_7_PIN
#define NODE_OUTPUT_7_PIN Output_7_Pin
#endif
#endif

#if NODE_OUTPUT_COUNT >= 8U
#ifndef NODE_OUTPUT_8_ID
#define NODE_OUTPUT_8_ID 8U
#endif

#ifndef NODE_OUTPUT_8_PORT
#define NODE_OUTPUT_8_PORT Output_8_GPIO_Port
#endif

#ifndef NODE_OUTPUT_8_PIN
#define NODE_OUTPUT_8_PIN Output_8_Pin
#endif
#endif

#ifndef NODE_OUTPUT_ACTIVE_STATE
#define NODE_OUTPUT_ACTIVE_STATE GPIO_PIN_SET
#endif

#ifndef NODE_OUTPUT_INACTIVE_STATE
#define NODE_OUTPUT_INACTIVE_STATE GPIO_PIN_RESET
#endif

#endif
