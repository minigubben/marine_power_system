#include "node_bus.h"

#include "main.h"

#include <string.h>

static UART_HandleTypeDef *node_uart = NULL;
static protocol_parser_t node_parser;
static uint8_t node_rx_byte = 0U;
static volatile bool node_frame_pending = false;
static protocol_frame_t node_pending_frame;

static bool node_start_receive(void)
{
    if (node_uart == NULL)
    {
        return false;
    }

    return HAL_UART_Receive_IT(node_uart, &node_rx_byte, 1U) == HAL_OK;
}

static bool node_wait_for_tx_complete(uint32_t timeout_ms)
{
    const uint32_t start_tick = HAL_GetTick();

    while (__HAL_UART_GET_FLAG(node_uart, UART_FLAG_TC) == RESET)
    {
        if ((HAL_GetTick() - start_tick) > timeout_ms)
        {
            return false;
        }
    }

    return true;
}

static bool node_bus_send_frame(uint8_t command, const uint8_t *payload, uint8_t payload_length)
{
    uint8_t frame[PROTOCOL_MAX_FRAME_LENGTH];
    const size_t frame_length =
        protocol_build_frame(command, payload, payload_length, frame, sizeof(frame));

    if ((node_uart == NULL) || (frame_length == 0U))
    {
        return false;
    }

    HAL_GPIO_WritePin(RS485_TX_EN_GPIO_Port, RS485_TX_EN_Pin, GPIO_PIN_SET);

    if (HAL_UART_Transmit(node_uart, frame, (uint16_t)frame_length, 100U) != HAL_OK)
    {
        HAL_GPIO_WritePin(RS485_TX_EN_GPIO_Port, RS485_TX_EN_Pin, GPIO_PIN_RESET);
        node_start_receive();
        return false;
    }

    if (!node_wait_for_tx_complete(100U))
    {
        HAL_GPIO_WritePin(RS485_TX_EN_GPIO_Port, RS485_TX_EN_Pin, GPIO_PIN_RESET);
        node_start_receive();
        return false;
    }

    HAL_GPIO_WritePin(RS485_TX_EN_GPIO_Port, RS485_TX_EN_Pin, GPIO_PIN_RESET);
    node_start_receive();
    return true;
}

void node_bus_init(UART_HandleTypeDef *huart)
{
    node_uart = huart;
    protocol_parser_init(&node_parser);
    memset((void *)&node_pending_frame, 0, sizeof(node_pending_frame));
    node_frame_pending = false;
    node_start_receive();
}

bool node_bus_take_received_frame(protocol_frame_t *frame_out)
{
    bool has_frame = false;

    if (frame_out == NULL)
    {
        return false;
    }

    __disable_irq();
    if (node_frame_pending)
    {
        *frame_out = node_pending_frame;
        node_frame_pending = false;
        has_frame = true;
    }
    __enable_irq();

    return has_frame;
}

bool node_bus_send_button_pressed(node_id_t node_id, button_id_t button_id)
{
    uint8_t payload[2];

    payload[0] = node_id;
    payload[1] = button_id;
    return node_bus_send_frame(PROTOCOL_CMD_BUTTON_PRESSED, payload, sizeof(payload));
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    protocol_frame_t frame;

    if ((node_uart == NULL) || (huart != node_uart))
    {
        return;
    }

    if (protocol_parser_push_byte(&node_parser, node_rx_byte, &frame) != 0)
    {
        node_pending_frame = frame;
        node_frame_pending = true;
    }

    node_start_receive();
}
