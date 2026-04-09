#ifndef NODE_INPUTS_H
#define NODE_INPUTS_H

#include <stdbool.h>

#include "shared/protocol.h"

void node_inputs_init(void);
bool node_inputs_poll_button_press(button_id_t *button_id_out);

#endif
