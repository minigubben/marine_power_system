#ifndef SCENES_H
#define SCENES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint8_t source_node_id;
    uint8_t button_id;
    uint8_t scene_id;
} ButtonBinding;

typedef struct
{
    uint8_t target_node_id;
    uint8_t output_id;
} SceneOutput;

void scenes_init(void);
bool scenes_find_binding(uint8_t source_node_id, uint8_t button_id, uint8_t *scene_id_out);
bool scenes_toggle(uint8_t scene_id);
bool scenes_get_state(uint8_t scene_id);
const SceneOutput *scenes_get_outputs(uint8_t scene_id, size_t *count_out);

#endif
