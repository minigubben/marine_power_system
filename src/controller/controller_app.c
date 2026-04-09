#include "controller_app.h"

#include "controller_bus.h"
#include "controller_inputs.h"
#include "scene_engine.h"

void controller_app_init(UART_HandleTypeDef *huart)
{
    controller_bus_init(huart);
    scene_engine_init();
    controller_inputs_init();
}

void controller_app_process(void)
{
    node_id_t source_node_id = 0U;
    button_id_t button_id = 0U;

    if (controller_inputs_poll_button_press(&source_node_id, &button_id))
    {
        scene_engine_handle_trigger(source_node_id, button_id);
    }
}
