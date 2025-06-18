#include "main.h"
#include "api.h"
#include "robot_config.h"


bool init_called = false;

void generic_init()
{
    if (init_called) return;
    init_called = true;

    device_init();
}

void competition_initialize()
{
    generic_init();
}


void initialize() {
    generic_init();
}
