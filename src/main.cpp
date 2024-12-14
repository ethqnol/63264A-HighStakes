#include "main.h"
#include "globals.h"
#include "pros/misc.h"

#define WLOWP_THRESH 40 // Low pressure warning threshold.
const int DRIVE_SPEED = 110;
const int TURN_SPEED = 90;
const int SWING_SPEED = 90;

using namespace ez;

Drive chassis({-14, -15, -16}, // Left Motor Ports
              {11, 12, 13},
              // Right Motor Ports
              7,    // IMU Port
              3.25, // Wheel Size
              600,  // rpm
              1.3333);
// Number of times the piston has been fired since air tank was filled.
int mogo_c = 0;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

    pros::delay(
        500); // Stop the user from doing anything while legacy ports configure

    chassis.opcontrol_curve_buttons_toggle(
        false); // Enables modifying the controller curve with buttons on the
                // joysticks
    chassis.opcontrol_drive_activebrake_set(
        2); // Sets the active brake kP. We recommend 2.
    chassis.opcontrol_curve_default_set(1, 5);
    default_constants(); // Set the drive to your own constants from autons.cpp!

    chassis.initialize();
    as::initialize();
    master.rumble("...");
}

void disabled() {}

void competition_initialize() {}

void autonomous() {

    /**
    UNCOMMENT AUTON ROUTINE
     */

    // red_auton_top();
    // red_rush();
    // blue_auton_top();
    // blue_rush();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

    chassis.drive_brake_set(MOTOR_BRAKE_COAST);
    // swing_example();
    while (true) {

        if (!pros::competition::is_connected()) {
            if (master.get_digital_new_press(DIGITAL_X))
                chassis.pid_tuner_toggle();

            // This code cost the lives of twelve orphans.
            if (master.get_digital_new_press(DIGITAL_B)) autonomous();

            chassis.pid_tuner_iterate(); // Allow PID Tuner to iterate
        }

        if (master.get_digital(DIGITAL_R1)) {
            intake.move(-127);
        } else if (master.get_digital(DIGITAL_R2)) {
            intake.move(127);
        } else {
            hook_conveyor.move(0);
        }

        if (master.get_digital(DIGITAL_L1)) {
            intake.move(127);
            hook_conveyor.move(-127);
        } else if (master.get_digital(DIGITAL_L2)) {
            intake.move(-127);
            hook_conveyor.move(127);
        } else {
            intake.move(0);
        }

        mogo_mech.button_toggle(master.get_digital(DIGITAL_A));

        if (master.get_digital_new_press(DIGITAL_A)) {
            if (mogo_c == 40) {
                master.rumble("-"); // Warn pilot of diminishing air pressure.
                pros::delay(10);
                master.print(0, 0, "ACT:%02d / 40!!", ++mogo_c);
            } else {
                master.print(0, 0, "ACT:%02d / 40", ++mogo_c);
            }
        }

        // Reset actuation counter (when tank refilled).
        if (master.get_digital_new_press(DIGITAL_Y) &&
            master.get_digital(DIGITAL_A))
            master.print(0, 0, "ACT:%02d / 40    ", mogo_c = 0);

        chassis.opcontrol_arcade_standard(SPLIT); // Arcade control

        pros::delay(util::DELAY_TIME); // This is used for timer calculations!
                                       // Keep this util::DELAY_TIME
    }
}
