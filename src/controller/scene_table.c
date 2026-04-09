#include "scene_table.h"

#include "input_config.h"

typedef struct
{
    uint8_t scene_id;
    bool is_active;
    const SceneOutput *outputs;
    size_t output_count;
} SceneDefinition;

static const ButtonBinding button_bindings[] = {
    {CONTROLLER_INPUT_SOURCE_NODE_ID, CONTROLLER_BUTTON_1_ID, 1U},
};

static const SceneOutput scene_1_outputs[] = {
    {10U, 1U},
    {10U, 2U},
    {11U, 1U},
};

static SceneDefinition scene_definitions[] = {
    {1U, false, scene_1_outputs, sizeof(scene_1_outputs) / sizeof(scene_1_outputs[0])},
};

static SceneDefinition *scene_table_find_definition(uint8_t scene_id)
{
    for (size_t i = 0U; i < (sizeof(scene_definitions) / sizeof(scene_definitions[0])); ++i)
    {
        if (scene_definitions[i].scene_id == scene_id)
        {
            return &scene_definitions[i];
        }
    }

    return NULL;
}

void scene_table_init(void)
{
    for (size_t i = 0U; i < (sizeof(scene_definitions) / sizeof(scene_definitions[0])); ++i)
    {
        scene_definitions[i].is_active = false;
    }
}

bool scene_table_find_binding(node_id_t source_node_id, button_id_t button_id, uint8_t *scene_id_out)
{
    for (size_t i = 0U; i < (sizeof(button_bindings) / sizeof(button_bindings[0])); ++i)
    {
        if ((button_bindings[i].source_node_id == source_node_id) &&
            (button_bindings[i].button_id == button_id))
        {
            if (scene_id_out != NULL)
            {
                *scene_id_out = button_bindings[i].scene_id;
            }

            return true;
        }
    }

    return false;
}

bool scene_table_toggle(uint8_t scene_id)
{
    SceneDefinition *scene = scene_table_find_definition(scene_id);

    if (scene == NULL)
    {
        return false;
    }

    scene->is_active = !scene->is_active;
    return true;
}

bool scene_table_get_state(uint8_t scene_id)
{
    SceneDefinition *scene = scene_table_find_definition(scene_id);

    if (scene == NULL)
    {
        return false;
    }

    return scene->is_active;
}

const SceneOutput *scene_table_get_outputs(uint8_t scene_id, size_t *count_out)
{
    SceneDefinition *scene = scene_table_find_definition(scene_id);

    if (count_out != NULL)
    {
        *count_out = 0U;
    }

    if (scene == NULL)
    {
        return NULL;
    }

    if (count_out != NULL)
    {
        *count_out = scene->output_count;
    }

    return scene->outputs;
}
