#include "controller_inputs.h"

#include "input_config.h"
#include "main.h"

static GPIO_PinState controller_last_raw_button_state = CONTROLLER_BUTTON_1_ACTIVE_STATE;
static GPIO_PinState controller_stable_button_state = CONTROLLER_BUTTON_1_ACTIVE_STATE;
static uint32_t controller_last_button_change_tick = 0U;

static GPIO_PinState controller_inputs_read_button(void)
{
    return HAL_GPIO_ReadPin(CONTROLLER_BUTTON_1_GPIO_Port, CONTROLLER_BUTTON_1_Pin);
}

void controller_inputs_init(void)
{
    GPIO_InitTypeDef gpio_init = {0};

    gpio_init.Pin = CONTROLLER_BUTTON_1_Pin;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = CONTROLLER_BUTTON_1_PULL_MODE;
    HAL_GPIO_Init(CONTROLLER_BUTTON_1_GPIO_Port, &gpio_init);
}

bool controller_inputs_poll_button_press(node_id_t *source_node_id_out, button_id_t *button_id_out)
{
    const GPIO_PinState raw_state = controller_inputs_read_button();

    if (raw_state != controller_last_raw_button_state)
    {
        controller_last_raw_button_state = raw_state;
        controller_last_button_change_tick = HAL_GetTick();
    }

    if ((raw_state != controller_stable_button_state) &&
        ((HAL_GetTick() - controller_last_button_change_tick) >= CONTROLLER_BUTTON_DEBOUNCE_MS))
    {
        controller_stable_button_state = raw_state;

        if (controller_stable_button_state == CONTROLLER_BUTTON_1_ACTIVE_STATE)
        {
            if (source_node_id_out != NULL)
            {
                *source_node_id_out = CONTROLLER_INPUT_SOURCE_NODE_ID;
            }

            if (button_id_out != NULL)
            {
                *button_id_out = CONTROLLER_BUTTON_1_ID;
            }
        }

        return controller_stable_button_state == CONTROLLER_BUTTON_1_ACTIVE_STATE;
    }

    return false;
}
