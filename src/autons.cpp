#include "globals.h"
#include "main.h"

/////
// For installation, upgrading, documentations and tutorials, check out our
// website! https://ez-robotics.github.io/EZ-Template/
/////

// These are out of 127
const int DRIVE_SPEED = 120;
const int TURN_SPEED = 92;
const int SWING_SPEED = 92;

///
// Constants
///
void default_constants() {
    chassis.pid_heading_constants_set(3, 0, 20);
    chassis.pid_drive_constants_set(7, 10, 80);
    chassis.pid_turn_constants_set(3, 0, 20);
    chassis.pid_swing_constants_set(5, 0, 30);

    chassis.pid_turn_exit_condition_set(300_ms, 3_deg, 500_ms, 7_deg, 750_ms,
                                        750_ms);
    chassis.pid_swing_exit_condition_set(300_ms, 3_deg, 500_ms, 7_deg, 750_ms,
                                         750_ms);
    chassis.pid_drive_exit_condition_set(300_ms, 1_in, 500_ms, 3_in, 750_ms,
                                         750_ms);

    chassis.slew_drive_constants_set(7_in, 80);
}

/**
Reseting Auton
 */
void reset_sensors() {
    chassis.pid_targets_reset();
    chassis.drive_imu_reset();
    chassis.drive_sensor_reset();
    intake.move_voltage(0);
    hook_conveyor.move_voltage(0);
}

/**
Red Auton
 */

void red_auton_top() {
    reset_sensors();
    mogo_mech.set(true);
    chassis.pid_drive_set(-43_in, DRIVE_SPEED);
    chassis.pid_wait();

    // pickup mogo
    mogo_mech.set(false);

    // score preload on mogo
    intake.move_voltage(-12000);
    hook_conveyor.move_voltage(12000);
    pros::delay(900);
    intake.move_voltage(0);
    hook_conveyor.move_voltage(0);

    // move to double stacked rings
    chassis.pid_turn_set(80_deg, TURN_SPEED);
    chassis.pid_wait();
    chassis.pid_drive_set(18.5_in, DRIVE_SPEED);
    chassis.pid_wait();

    // intake/score mogo
    intake.move_voltage(-12000);
    hook_conveyor.move_voltage(12000);
    pros::delay(200);
    chassis.pid_wait();
    chassis.pid_turn_set(-90_deg, TURN_SPEED);
    chassis.pid_wait();
    intake.move_voltage(0);
    hook_conveyor.move_voltage(0);
    chassis.pid_drive_set(38_in, TURN_SPEED);
    chassis.pid_wait();
}

void blue_auton_top() {
    reset_sensors();
    mogo_mech.set(true);
    chassis.pid_drive_set(-43_in, DRIVE_SPEED);
    chassis.pid_wait();

    // pickup mogo
    mogo_mech.set(false);

    // score preload on mogo
    intake.move_voltage(-12000);
    hook_conveyor.move_voltage(12000);
    pros::delay(900);
    intake.move_voltage(0);
    hook_conveyor.move_voltage(0);

    // move to double stacked rings
    chassis.pid_turn_set(-80_deg, TURN_SPEED);
    chassis.pid_wait();
    chassis.pid_drive_set(18.5_in, DRIVE_SPEED);
    chassis.pid_wait();

    // intake/score mogo
    intake.move_voltage(-12000);
    hook_conveyor.move_voltage(12000);
    pros::delay(200);
    chassis.pid_wait();
    chassis.pid_turn_set(90_deg, TURN_SPEED);
    chassis.pid_wait();
    intake.move_voltage(0);
    hook_conveyor.move_voltage(0);
    chassis.pid_drive_set(38_in, TURN_SPEED);
    chassis.pid_wait();
}

void red_rush() {
    reset_sensors();
    intake.move_voltage(-12000);
    hook_conveyor.move_voltage(12000);
    pros::delay(900);
    mogo_mech.set(true);
    intake.move_voltage(0);
    hook_conveyor.move_voltage(0);
    chassis.pid_drive_set(-43_in, DRIVE_SPEED);
    chassis.pid_wait();
    mogo_mech.set(false);
    pros::delay(200);
    chassis.pid_turn_set(-80_deg, TURN_SPEED);
    chassis.pid_wait();
    chassis.pid_drive_set(18.5_in, DRIVE_SPEED);
    chassis.pid_wait();
    intake.move_voltage(-12000);
    hook_conveyor.move_voltage(12000);
    pros::delay(900);
    chassis.pid_wait();
    chassis.pid_turn_set(90_deg, TURN_SPEED);
    chassis.pid_wait();
    intake.move_voltage(0);
    hook_conveyor.move_voltage(0);
    chassis.pid_drive_set(38_in, TURN_SPEED);
    chassis.pid_wait();
}

void blue_rush() {
    reset_sensors();
    mogo_mech.set(true);
    chassis.pid_drive_set(-43_in, DRIVE_SPEED);
    chassis.pid_wait();

    // pickup mogo
    mogo_mech.set(false);

    // score preload on mogo
    intake.move_voltage(-12000);
    hook_conveyor.move_voltage(12000);
    pros::delay(900);
    intake.move_voltage(0);
    hook_conveyor.move_voltage(0);

    // move to double stacked rings
    chassis.pid_turn_set(80_deg, TURN_SPEED);
    chassis.pid_wait();
    chassis.pid_drive_set(18.5_in, DRIVE_SPEED);
    chassis.pid_wait();

    // intake/score mogo
    intake.move_voltage(-12000);
    hook_conveyor.move_voltage(12000);
    pros::delay(200);
    chassis.pid_wait();
    chassis.pid_turn_set(-90_deg, TURN_SPEED);
    chassis.pid_wait();
    intake.move_voltage(0);
    hook_conveyor.move_voltage(0);
    chassis.pid_drive_set(38_in, TURN_SPEED);
    chassis.pid_wait();
}

void skills() {
    hook_conveyor.move_voltage(12000);
    pros::delay(300);
    hook_conveyor.move_voltage(-12000);
    pros::delay(100);
    chassis.pid_drive_set(18.5_in, DRIVE_SPEED);
}

void tug(int attempts) {
    for (int i = 0; i < attempts - 1; i++) {
        // Attempt to drive backwards
        printf("i - %i", i);
        chassis.pid_drive_set(-12_in, 127);
        chassis.pid_wait();

        // If failsafed...
        if (chassis.interfered) {
            chassis.drive_sensor_reset();
            chassis.pid_drive_set(-2_in, 20);
            pros::delay(1000);
        }
        // If robot successfully drove back, return
        else {
            return;
        }
    }
}

// If there is no interference, robot will drive forward and turn 90 degrees.
// If interfered, robot will drive forward and then attempt to drive backwards.
void interfered_example() {
    chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
    chassis.pid_wait();

    if (chassis.interfered) {
        tug(3);
        return;
    }

    chassis.pid_turn_set(90_deg, TURN_SPEED);
    chassis.pid_wait();
}

// . . .
// Make your own autonomous functions here!
// . . .
/*
// Move motor m speed s for t miliseconds.
void mvmt(pros::Motor m, int s, int t) {
    m.move(s);
    pros::delay(t);
    m.move(0);
}

void skills() {
    // Start against wall with wall stake.
    // Trigger conveyor to score the preload donut.
    //mvmt(hook_conveyor, 127, 1000);
    // Move forward a bit.
    chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
}
*/
