#ifndef SCENE_ENGINE_H
#define SCENE_ENGINE_H

#include "shared/protocol.h"

void scene_engine_init(void);
void scene_engine_handle_trigger(node_id_t source_node_id, button_id_t button_id);

#endif
