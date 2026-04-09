#include "node_outputs.h"

#include "main.h"
#include "node_config.h"

#include <stddef.h>

typedef struct
{
    output_id_t output_id;
    GPIO_TypeDef *port;
    uint16_t pin;
} OutputMapping;

static const OutputMapping node_output_mappings[] = {
    {NODE_OUTPUT_1_ID, NODE_OUTPUT_1_PORT, NODE_OUTPUT_1_PIN},
    {NODE_OUTPUT_2_ID, NODE_OUTPUT_2_PORT, NODE_OUTPUT_2_PIN},
    {NODE_OUTPUT_3_ID, NODE_OUTPUT_3_PORT, NODE_OUTPUT_3_PIN},
    {NODE_OUTPUT_4_ID, NODE_OUTPUT_4_PORT, NODE_OUTPUT_4_PIN},
    {NODE_OUTPUT_5_ID, NODE_OUTPUT_5_PORT, NODE_OUTPUT_5_PIN},
    {NODE_OUTPUT_6_ID, NODE_OUTPUT_6_PORT, NODE_OUTPUT_6_PIN},
};

void node_outputs_init(void)
{
}

void node_outputs_set_state(output_id_t output_id, bool state)
{
    if (NODE_HAS_OUTPUTS == 0U)
    {
        return;
    }

    for (size_t i = 0U; i < (sizeof(node_output_mappings) / sizeof(node_output_mappings[0])); ++i)
    {
        if (node_output_mappings[i].output_id == output_id)
        {
            HAL_GPIO_WritePin(
                node_output_mappings[i].port,
                node_output_mappings[i].pin,
                state ? NODE_OUTPUT_ACTIVE_STATE : NODE_OUTPUT_INACTIVE_STATE);
            return;
        }
    }
}
