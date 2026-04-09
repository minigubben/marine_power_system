#include "scene_engine.h"

#include "controller_bus.h"
#include "scene_table.h"

void scene_engine_init(void)
{
    scene_table_init();
}

void scene_engine_handle_trigger(node_id_t source_node_id, button_id_t button_id)
{
    uint8_t scene_id = 0U;
    size_t output_count = 0U;
    bool scene_active = false;
    const SceneOutput *outputs = NULL;

    if (!scene_table_find_binding(source_node_id, button_id, &scene_id))
    {
        return;
    }

    if (!scene_table_toggle(scene_id))
    {
        return;
    }

    scene_active = scene_table_get_state(scene_id);
    outputs = scene_table_get_outputs(scene_id, &output_count);

    if (outputs == NULL)
    {
        return;
    }

    for (size_t i = 0U; i < output_count; ++i)
    {
        controller_bus_send_output_state(
            outputs[i].target_node_id,
            outputs[i].output_id,
            scene_active);
    }
}
