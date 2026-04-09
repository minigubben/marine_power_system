#ifndef SCENE_TABLE_H
#define SCENE_TABLE_H

#include <stdbool.h>
#include <stddef.h>

#include "shared/protocol.h"

typedef struct
{
    node_id_t source_node_id;
    button_id_t button_id;
    uint8_t scene_id;
} ButtonBinding;

typedef struct
{
    node_id_t target_node_id;
    output_id_t output_id;
} SceneOutput;

void scene_table_init(void);
bool scene_table_find_binding(node_id_t source_node_id, button_id_t button_id, uint8_t *scene_id_out);
bool scene_table_toggle(uint8_t scene_id);
bool scene_table_get_state(uint8_t scene_id);
const SceneOutput *scene_table_get_outputs(uint8_t scene_id, size_t *count_out);

#endif
