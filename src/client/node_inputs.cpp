#include "node_inputs.h"

#include "main.h"
#include "node_config.h"

#include <stddef.h>

typedef struct
{
    button_id_t button_id;
    GPIO_TypeDef *port;
    uint16_t pin;
    uint32_t pull_mode;
    GPIO_PinState active_state;
} NodeInputMapping;

#if NODE_HAS_INPUTS != 0U
static const NodeInputMapping node_input_mappings[] = {
    {NODE_BUTTON_1_ID, NODE_BUTTON_1_GPIO_Port, NODE_BUTTON_1_Pin, NODE_BUTTON_1_PULL_MODE, NODE_BUTTON_1_ACTIVE_STATE},
    {NODE_BUTTON_2_ID, NODE_BUTTON_2_GPIO_Port, NODE_BUTTON_2_Pin, NODE_BUTTON_2_PULL_MODE, NODE_BUTTON_2_ACTIVE_STATE},
    {NODE_BUTTON_3_ID, NODE_BUTTON_3_GPIO_Port, NODE_BUTTON_3_Pin, NODE_BUTTON_3_PULL_MODE, NODE_BUTTON_3_ACTIVE_STATE},
    {NODE_BUTTON_4_ID, NODE_BUTTON_4_GPIO_Port, NODE_BUTTON_4_Pin, NODE_BUTTON_4_PULL_MODE, NODE_BUTTON_4_ACTIVE_STATE},
    {NODE_BUTTON_5_ID, NODE_BUTTON_5_GPIO_Port, NODE_BUTTON_5_Pin, NODE_BUTTON_5_PULL_MODE, NODE_BUTTON_5_ACTIVE_STATE},
    {NODE_BUTTON_6_ID, NODE_BUTTON_6_GPIO_Port, NODE_BUTTON_6_Pin, NODE_BUTTON_6_PULL_MODE, NODE_BUTTON_6_ACTIVE_STATE},
};

static GPIO_PinState node_last_raw_button_states[sizeof(node_input_mappings) / sizeof(node_input_mappings[0])];
static GPIO_PinState node_stable_button_states[sizeof(node_input_mappings) / sizeof(node_input_mappings[0])];
static uint32_t node_last_button_change_ticks[sizeof(node_input_mappings) / sizeof(node_input_mappings[0])];

static GPIO_PinState node_inputs_read_button(size_t index)
{
    return HAL_GPIO_ReadPin(node_input_mappings[index].port, node_input_mappings[index].pin);
}
#endif

void node_inputs_init(void)
{
    if (NODE_HAS_INPUTS == 0U)
    {
        return;
    }

#if NODE_HAS_INPUTS != 0U
    GPIO_InitTypeDef gpio_init = {0};

    for (size_t i = 0U; i < (sizeof(node_input_mappings) / sizeof(node_input_mappings[0])); ++i)
    {
        gpio_init.Pin = node_input_mappings[i].pin;
        gpio_init.Mode = GPIO_MODE_INPUT;
        gpio_init.Pull = node_input_mappings[i].pull_mode;
        HAL_GPIO_Init(node_input_mappings[i].port, &gpio_init);

        node_last_raw_button_states[i] = node_inputs_read_button(i);
        node_stable_button_states[i] = node_last_raw_button_states[i];
        node_last_button_change_ticks[i] = HAL_GetTick();
    }
#endif
}

bool node_inputs_poll_button_press(button_id_t *button_id_out)
{
    if (NODE_HAS_INPUTS == 0U)
    {
        return false;
    }

#if NODE_HAS_INPUTS != 0U
    for (size_t i = 0U; i < (sizeof(node_input_mappings) / sizeof(node_input_mappings[0])); ++i)
    {
        const GPIO_PinState raw_state = node_inputs_read_button(i);

        if (raw_state != node_last_raw_button_states[i])
        {
            node_last_raw_button_states[i] = raw_state;
            node_last_button_change_ticks[i] = HAL_GetTick();
        }

        if ((raw_state != node_stable_button_states[i]) &&
            ((HAL_GetTick() - node_last_button_change_ticks[i]) >= NODE_BUTTON_DEBOUNCE_MS))
        {
            node_stable_button_states[i] = raw_state;

            if ((node_stable_button_states[i] == node_input_mappings[i].active_state) &&
                (button_id_out != NULL))
            {
                *button_id_out = node_input_mappings[i].button_id;
            }

            return node_stable_button_states[i] == node_input_mappings[i].active_state;
        }
    }
#endif

    return false;
}
