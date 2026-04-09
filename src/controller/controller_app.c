#include "controller_app.h"

#include "main.h"
#include "scenes.h"
#include "shared/protocol.h"

#include <stdbool.h>
#include <string.h>

static UART_HandleTypeDef *controller_uart = NULL;
static protocol_parser_t controller_parser;
static uint8_t controller_rx_byte = 0U;
static volatile bool controller_frame_pending = false;
static protocol_frame_t controller_pending_frame;

static bool controller_start_receive(void)
{
    if (controller_uart == NULL)
    {
        return false;
    }

    return HAL_UART_Receive_IT(controller_uart, &controller_rx_byte, 1U) == HAL_OK;
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

static void controller_dispatch_frame(const protocol_frame_t *frame)
{
    if ((frame->command == PROTOCOL_CMD_BUTTON_PRESSED) && (frame->payload_length == 2U))
    {
        uint8_t scene_id = 0U;

        if (!scenes_find_binding(frame->payload[0], frame->payload[1], &scene_id))
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
}

void controller_app_init(UART_HandleTypeDef *huart)
{
    controller_uart = huart;
    protocol_parser_init(&controller_parser);
    memset((void *)&controller_pending_frame, 0, sizeof(controller_pending_frame));
    controller_frame_pending = false;
    scenes_init();
    controller_start_receive();
}

void controller_app_process(void)
{
    protocol_frame_t frame;
    bool has_frame = false;

    __disable_irq();
    if (controller_frame_pending)
    {
        frame = controller_pending_frame;
        controller_frame_pending = false;
        has_frame = true;
    }
    __enable_irq();

    if (has_frame)
    {
        controller_dispatch_frame(&frame);
    }
}

void controller_app_rx_complete_callback(UART_HandleTypeDef *huart)
{
    protocol_frame_t frame;

    if ((controller_uart == NULL) || (huart != controller_uart))
    {
        return;
    }

    if (protocol_parser_push_byte(&controller_parser, controller_rx_byte, &frame) != 0)
    {
        controller_pending_frame = frame;
        controller_frame_pending = true;
    }

    controller_start_receive();
}
