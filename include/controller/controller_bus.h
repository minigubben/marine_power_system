#ifndef CONTROLLER_BUS_H
#define CONTROLLER_BUS_H

#include <stdbool.h>

#include "shared/protocol.h"
#include "stm32f0xx_hal.h"

void controller_bus_init(UART_HandleTypeDef *huart);
bool controller_bus_send_frame(uint8_t command, const uint8_t *payload, uint8_t payload_length);
bool controller_bus_send_output_state(node_id_t target_node_id, output_id_t output_id, bool state);

#endif
