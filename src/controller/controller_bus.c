#include "controller_bus.h"

#include "main.h"

static UART_HandleTypeDef *controller_uart = NULL;

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

void controller_bus_init(UART_HandleTypeDef *huart)
{
    controller_uart = huart;
}

bool controller_bus_send_frame(uint8_t command, const uint8_t *payload, uint8_t payload_length)
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

bool controller_bus_send_output_state(node_id_t target_node_id, output_id_t output_id, bool state)
{
    uint8_t payload[3];

    payload[0] = target_node_id;
    payload[1] = output_id;
    payload[2] = state ? 1U : 0U;
    return controller_bus_send_frame(PROTOCOL_CMD_SET_OUTPUT_STATE, payload, sizeof(payload));
}
