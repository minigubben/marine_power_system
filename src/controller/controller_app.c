#include "controller_app.h"

#include "input_config.h"
#include "main.h"
#include "scenes.h"
#include "shared/protocol.h"

#include <stdbool.h>

static UART_HandleTypeDef *controller_uart = NULL;
static GPIO_PinState controller_last_raw_button_state = CONTROLLER_BUTTON_1_ACTIVE_STATE;
static GPIO_PinState controller_stable_button_state = CONTROLLER_BUTTON_1_ACTIVE_STATE;
static uint32_t controller_last_button_change_tick = 0U;

static void controller_init_button(void)
{
    GPIO_InitTypeDef gpio_init = {0};

    gpio_init.Pin = CONTROLLER_BUTTON_1_Pin;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = CONTROLLER_BUTTON_1_PULL_MODE;
    HAL_GPIO_Init(CONTROLLER_BUTTON_1_GPIO_Port, &gpio_init);
}

static GPIO_PinState controller_button_read(void)
{
    return HAL_GPIO_ReadPin(CONTROLLER_BUTTON_1_GPIO_Port, CONTROLLER_BUTTON_1_Pin);
}

static bool controller_poll_button_press(void)
{
    const GPIO_PinState raw_state = controller_button_read();

    if (raw_state != controller_last_raw_button_state)
    {
        controller_last_raw_button_state = raw_state;
        controller_last_button_change_tick = HAL_GetTick();
    }

    if ((raw_state != controller_stable_button_state) &&
        ((HAL_GetTick() - controller_last_button_change_tick) >= CONTROLLER_BUTTON_DEBOUNCE_MS))
    {
        controller_stable_button_state = raw_state;
        return controller_stable_button_state == CONTROLLER_BUTTON_1_ACTIVE_STATE;
    }

    return false;
}

static bool controller_wait_for_tx_complete(uint32_t timeout_ms)
{
    const uint32_t start_tick = HAL_GetTick();

    while (__HAL_UART_GET_FLAG(controller_uart, UART_FLAG_TC) == RESET)
    {
        if ((HAL_GetTick() - start_tick) > timeout_ms)
        {
            return false;
        }
    }

    return true;
}

static bool controller_send_frame(uint8_t command, const uint8_t *payload, uint8_t payload_length)
{
    uint8_t frame[PROTOCOL_MAX_FRAME_LENGTH];
    const size_t frame_length =
        protocol_build_frame(command, payload, payload_length, frame, sizeof(frame));

    if ((controller_uart == NULL) || (frame_length == 0U))
    {
        return false;
    }

    HAL_GPIO_WritePin(RS485_TX_EN_GPIO_Port, RS485_TX_EN_Pin, GPIO_PIN_SET);

    if (HAL_UART_Transmit(controller_uart, frame, (uint16_t)frame_length, 100U) != HAL_OK)
    {
        HAL_GPIO_WritePin(RS485_TX_EN_GPIO_Port, RS485_TX_EN_Pin, GPIO_PIN_RESET);
        return false;
    }

    if (!controller_wait_for_tx_complete(100U))
    {
        HAL_GPIO_WritePin(RS485_TX_EN_GPIO_Port, RS485_TX_EN_Pin, GPIO_PIN_RESET);
        return false;
    }

    HAL_GPIO_WritePin(RS485_TX_EN_GPIO_Port, RS485_TX_EN_Pin, GPIO_PIN_RESET);
    return true;
}

static void controller_handle_scene_trigger(uint8_t source_node_id, uint8_t button_id)
{
    uint8_t scene_id = 0U;

    if (!scenes_find_binding(source_node_id, button_id, &scene_id))
    {
        return;
    }

    if (!scenes_toggle(scene_id))
    {
        return;
    }

    {
        const bool scene_active = scenes_get_state(scene_id);
        size_t output_count = 0U;
        const SceneOutput *outputs = scenes_get_outputs(scene_id, &output_count);

        if (outputs == NULL)
        {
            return;
        }

        for (size_t i = 0U; i < output_count; ++i)
        {
            uint8_t payload[3];

            payload[0] = outputs[i].target_node_id;
            payload[1] = outputs[i].output_id;
            payload[2] = scene_active ? 1U : 0U;
            controller_send_frame(PROTOCOL_CMD_SET_OUTPUT_STATE, payload, sizeof(payload));
        }
    }
}

void controller_app_init(UART_HandleTypeDef *huart)
{
    controller_uart = huart;
    scenes_init();
    controller_init_button();
}

void controller_app_process(void)
{
    if (controller_poll_button_press())
    {
        controller_handle_scene_trigger(
            CONTROLLER_INPUT_SOURCE_NODE_ID,
            CONTROLLER_BUTTON_1_ID);
    }
}
