#include "node_app.h"

#include "node_bus.h"
#include "node_config.h"
#include "node_inputs.h"
#include "node_outputs.h"

static void node_app_handle_frame(const protocol_frame_t *frame)
{
    if (NODE_HAS_OUTPUTS == 0U)
    {
        return;
    }

    if ((frame->command != PROTOCOL_CMD_SET_OUTPUT_STATE) || (frame->payload_length != 3U))
    {
        return;
    }

    if (frame->payload[0] != NODE_ID)
    {
        return;
    }

    node_outputs_set_state(frame->payload[1], frame->payload[2] != 0U);
}

void node_app_init(UART_HandleTypeDef *huart)
{
    node_outputs_init();
    node_inputs_init();
    node_bus_init(huart);
}

void node_app_process(void)
{
    protocol_frame_t frame;
    button_id_t button_id = 0U;

    if (node_bus_take_received_frame(&frame))
    {
        node_app_handle_frame(&frame);
    }

    if ((NODE_HAS_INPUTS != 0U) && node_inputs_poll_button_press(&button_id))
    {
        node_bus_send_button_pressed(NODE_ID, button_id);
    }

    HAL_Delay(10U);
}
