#ifndef NODE_OUTPUTS_H
#define NODE_OUTPUTS_H

#include <stdbool.h>

#include "shared/protocol.h"

void node_outputs_init(void);
void node_outputs_set_state(output_id_t output_id, bool state);

#endif
