#include "main.h"
#include "api.h"
#include "robot_config.h"

bool init_called = false;

void generic_init() {
    if (init_called) return;
    init_called = true;

    device_init();
}

void competition_initialize() {
    generic_init();
}


void initialize() {
    generic_init();
}

void opcontrol() {
    logger.log({chisel::logger::LogLevel::Info, "Opcontrol started"});

    chassis.state = chisel::State::Driver;

    while (chassis.state == chisel::State::Driver) {

        // intake

        if (master.get_digital(intake_store_button)) intake_target = 1;
        if (master.get_digital(intake_high_button)) intake_target = 2;
        if (master.get_digital(intake_mid_button)) intake_target = 3;

        int32_t intake_mt = 0;
        if (master.get_digital(intake_fwd_button)) intake_mt = 1;
        if (master.get_digital(intake_rev_button)) intake_mt = -1;

        const int32_t d_intake_speed = intake_mt * INTAKE_SPEED;
        (void)intake_stage1.move(d_intake_speed * intake_stage1_mt[intake_target]);
        (void)intake_stage2.move(d_intake_speed * intake_stage2_mt[intake_target]);
        (void)intake_stage1.move(d_intake_speed * intake_stage1_mt[intake_target]);
    }
}
