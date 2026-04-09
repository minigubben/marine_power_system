#ifndef NODE_BUS_H
#define NODE_BUS_H

#include <stdbool.h>

#include "shared/protocol.h"
#include "stm32f0xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void node_bus_init(UART_HandleTypeDef *huart);
bool node_bus_take_received_frame(protocol_frame_t *frame_out);
bool node_bus_send_button_pressed(node_id_t node_id, button_id_t button_id);

#ifdef __cplusplus
}
#endif

#endif
