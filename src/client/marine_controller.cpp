#include "marine_controller.h"

#include "main.h"
#include "node_config.h"
#include "shared/protocol.h"

#include <stdbool.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

typedef struct
{
    output_id_t output_id;
    GPIO_TypeDef *port;
    uint16_t pin;
} OutputMapping;

static const OutputMapping client_outputs[] = {
    {1U, Output_1_GPIO_Port, Output_1_Pin},
    {2U, Output_2_GPIO_Port, Output_2_Pin},
    {3U, Output_3_GPIO_Port, Output_3_Pin},
};

static protocol_parser_t client_parser;
static uint8_t client_rx_byte = 0U;
static volatile bool client_frame_pending = false;
static protocol_frame_t client_pending_frame;

static bool client_start_receive(void)
{
    return HAL_UART_Receive_IT(&huart2, &client_rx_byte, 1U) == HAL_OK;
}

static void client_init_button(void)
{
    GPIO_InitTypeDef gpio_init = {0};

    if (CLIENT_ROLE != CLIENT_ROLE_INPUT)
    {
        return;
    }

    gpio_init.Pin = BUTTON_1_Pin;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = BUTTON_1_PULL_MODE;
    HAL_GPIO_Init(BUTTON_1_GPIO_Port, &gpio_init);
}

static GPIO_PinState client_button_read(void)
{
    return HAL_GPIO_ReadPin(BUTTON_1_GPIO_Port, BUTTON_1_Pin);
}

static bool client_wait_for_tx_complete(uint32_t timeout_ms)
{
    const uint32_t start_tick = HAL_GetTick();

    while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET)
    {
        if ((HAL_GetTick() - start_tick) > timeout_ms)
        {
            return false;
        }
    }

    return true;
}

static bool client_send_frame(uint8_t command, const uint8_t *payload, uint8_t payload_length)
{
    uint8_t frame[PROTOCOL_MAX_FRAME_LENGTH];
    const size_t frame_length =
        protocol_build_frame(command, payload, payload_length, frame, sizeof(frame));

    if (frame_length == 0U)
    {
        return false;
    }

    HAL_GPIO_WritePin(RS485_TX_EN_GPIO_Port, RS485_TX_EN_Pin, GPIO_PIN_SET);

    if (HAL_UART_Transmit(&huart2, frame, (uint16_t)frame_length, 100U) != HAL_OK)
    {
        HAL_GPIO_WritePin(RS485_TX_EN_GPIO_Port, RS485_TX_EN_Pin, GPIO_PIN_RESET);
        return false;
    }

    if (!client_wait_for_tx_complete(100U))
    {
        HAL_GPIO_WritePin(RS485_TX_EN_GPIO_Port, RS485_TX_EN_Pin, GPIO_PIN_RESET);
        return false;
    }

    HAL_GPIO_WritePin(RS485_TX_EN_GPIO_Port, RS485_TX_EN_Pin, GPIO_PIN_RESET);
    return true;
}

static void client_send_button_pressed(void)
{
    uint8_t payload[2];

    payload[0] = CLIENT_NODE_ID;
    payload[1] = BUTTON_1_ID;
    client_send_frame(PROTOCOL_CMD_BUTTON_PRESSED, payload, sizeof(payload));
}

static bool client_poll_button_press(void)
{
    static GPIO_PinState last_raw_state = BUTTON_1_ACTIVE_STATE;
    static GPIO_PinState stable_state = BUTTON_1_ACTIVE_STATE;
    static uint32_t last_change_tick = 0U;
    const GPIO_PinState raw_state = client_button_read();

    if (raw_state != last_raw_state)
    {
        last_raw_state = raw_state;
        last_change_tick = HAL_GetTick();
    }

    if ((raw_state != stable_state) &&
        ((HAL_GetTick() - last_change_tick) >= BUTTON_DEBOUNCE_MS))
    {
        stable_state = raw_state;
        return stable_state == BUTTON_1_ACTIVE_STATE;
    }

    return false;
}

static void client_set_output(output_id_t output_id, bool state)
{
    for (size_t i = 0U; i < (sizeof(client_outputs) / sizeof(client_outputs[0])); ++i)
    {
        if (client_outputs[i].output_id == output_id)
        {
            HAL_GPIO_WritePin(
                client_outputs[i].port,
                client_outputs[i].pin,
                state ? GPIO_PIN_SET : GPIO_PIN_RESET);
            return;
        }
    }
}

static void client_handle_frame(const protocol_frame_t *frame)
{
    if (CLIENT_ROLE != CLIENT_ROLE_OUTPUT)
    {
        return;
    }

    if ((frame->command != PROTOCOL_CMD_SET_OUTPUT_STATE) || (frame->payload_length != 3U))
    {
        return;
    }

    if (frame->payload[0] != CLIENT_NODE_ID)
    {
        return;
    }

    client_set_output(frame->payload[1], frame->payload[2] != 0U);
}

static void client_process_pending_frame(void)
{
    protocol_frame_t frame;
    bool has_frame = false;

    __disable_irq();
    if (client_frame_pending)
    {
        frame = client_pending_frame;
        client_frame_pending = false;
        has_frame = true;
    }
    __enable_irq();

    if (has_frame)
    {
        client_handle_frame(&frame);
    }
}

int marineMain(void)
{
    protocol_parser_init(&client_parser);
    memset((void *)&client_pending_frame, 0, sizeof(client_pending_frame));
    client_frame_pending = false;

    client_init_button();
    client_start_receive();

    while (1)
    {
        client_process_pending_frame();

        if ((CLIENT_ROLE == CLIENT_ROLE_INPUT) && client_poll_button_press())
        {
            client_send_button_pressed();
        }

        HAL_Delay(10U);
    }
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    protocol_frame_t frame;

    if (huart->Instance != USART2)
    {
        return;
    }

    if (protocol_parser_push_byte(&client_parser, client_rx_byte, &frame) != 0)
    {
        client_pending_frame = frame;
        client_frame_pending = true;
    }

    client_start_receive();
}
