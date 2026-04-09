#include "node_inputs.h"

#include "main.h"
#include "node_config.h"

static GPIO_PinState node_last_raw_button_state = NODE_BUTTON_1_ACTIVE_STATE;
static GPIO_PinState node_stable_button_state = NODE_BUTTON_1_ACTIVE_STATE;
static uint32_t node_last_button_change_tick = 0U;

static GPIO_PinState node_inputs_read_button(void)
{
    return HAL_GPIO_ReadPin(NODE_BUTTON_1_GPIO_Port, NODE_BUTTON_1_Pin);
}

void node_inputs_init(void)
{
    GPIO_InitTypeDef gpio_init = {0};

    if (NODE_HAS_INPUTS == 0U)
    {
        return;
    }

    gpio_init.Pin = NODE_BUTTON_1_Pin;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = NODE_BUTTON_1_PULL_MODE;
    HAL_GPIO_Init(NODE_BUTTON_1_GPIO_Port, &gpio_init);
}

bool node_inputs_poll_button_press(button_id_t *button_id_out)
{
    GPIO_PinState raw_state = GPIO_PIN_RESET;

    if (NODE_HAS_INPUTS == 0U)
    {
        return false;
    }

    raw_state = node_inputs_read_button();

    if (raw_state != node_last_raw_button_state)
    {
        node_last_raw_button_state = raw_state;
        node_last_button_change_tick = HAL_GetTick();
    }

    if ((raw_state != node_stable_button_state) &&
        ((HAL_GetTick() - node_last_button_change_tick) >= NODE_BUTTON_DEBOUNCE_MS))
    {
        node_stable_button_state = raw_state;

        if ((node_stable_button_state == NODE_BUTTON_1_ACTIVE_STATE) && (button_id_out != NULL))
        {
            *button_id_out = NODE_BUTTON_1_ID;
        }

        return node_stable_button_state == NODE_BUTTON_1_ACTIVE_STATE;
    }

    return false;
}
