#include "scenes.h"

struct Scenes
{
    int in_address[50]; //contains addresses that triggers the scene.
    int on_on_out_addresses[50]; //turns theeses addresses on when scene is activated.
    int on_off_out_addresses[50]; //turns theeses addresses off when scene is activated.
    int off_out_addresses[50]; //turns theeses addresses off when scene is deactivated.
};
