#ifndef CONTROLLER_INPUTS_H
#define CONTROLLER_INPUTS_H

#include <stdbool.h>

#include "shared/protocol.h"

void controller_inputs_init(void);
bool controller_inputs_poll_button_press(node_id_t *source_node_id_out, button_id_t *button_id_out);

#endif
