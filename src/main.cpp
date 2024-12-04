#include "main.h"


using namespace ez;


inline pros::Motor intake(2);
inline pros::Motor hook_conveyor(3);
inline ez::Piston mogo_mech('A');

Drive chassis (
    {-11, -18, -20}, //Left Motor Ports
    {1, 8 ,10},
    
    //Right Motor Ports
    7, //IMU Port
    3.25, //Wheel Size
    600, // rpm
    1.3333
);



/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {


    pros::delay(500); // Stop the user from doing anything while legacy ports configure


    chassis.opcontrol_curve_buttons_toggle(true); // Enables modifying the controller curve with buttons on the joysticks
    chassis.opcontrol_drive_activebrake_set(0); // Sets the active brake kP. We recommend 2.
    chassis.opcontrol_curve_default_set(0, 0); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
    default_constants(); // Set the drive to your own constants from autons.cpp!

    // as::auton_selector.autons_add({
    //     Auton("Example Drive\n\nDrive forward and come back.", drive_example),
    //     Auton("Example Turn\n\nTurn 3 times.", turn_example),
    //     Auton("Drive and Turn\n\nDrive forward, turn, come back. ", drive_and_turn),
    //     Auton("Drive and Turn\n\nSlow down during drive.", wait_until_change_speed),
    //     Auton("Swing Example\n\nSwing in an 'S' curve", swing_example),
    //     Auton("Combine all 3 movements", combining_movements),
    //     Auton("Interference\n\nAfter driving forward, robot performs differently if interfered or not.", interfered_example),
    // });


    chassis.initialize();
    as::initialize();
    master.rumble(".");
}


void disabled() {

}

void competition_initialize() {

}

void autonomous() {
    chassis.pid_targets_reset(); // Resets PID targets to 0
    chassis.drive_imu_reset(); // Reset gyro position to 0
    chassis.drive_sensor_reset(); // Reset drive sensors to 0
    chassis.drive_brake_set(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency


    int dist = 48;
    chassis.pid_drive_set(dist, 127, true);
    chassis.pid_wait_until(dist - 5);

    int dist2 = -(dist-6);
    chassis.pid_drive_set(dist2, 127, true);
    chassis.pid_wait_until(dist2 + 5);
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
    //swing_example();
    while (true) {
        
        

        if (!pros::competition::is_connected()) { 
            if (master.get_digital_new_press(DIGITAL_X)) 
                chassis.pid_tuner_toggle();
                

            if (master.get_digital_new_press(DIGITAL_B)) 
                autonomous();

            chassis.pid_tuner_iterate(); // Allow PID Tuner to iterate
        } 


        if (master.get_digital(DIGITAL_L1)) {
            intake.move(127);
        } 
        else if (master.get_digital(DIGITAL_L2)) {
            intake.move(-127);
        } 
        else {
            intake.move(0);
        }

        
        if (master.get_digital(DIGITAL_R1)) {
            hook_conveyor.move(127);
        } 
        else if (master.get_digital(DIGITAL_R2)) {
            hook_conveyor.move(-127);
        } 
        else {
            hook_conveyor.move(0);
        }

       // mogo_mech.button_toggle(master.get_digital(DIGITAL_A)) // 'A' button for mogo


        chassis.opcontrol_arcade_standard(SPLIT); // Arcade control


        pros::delay(util::DELAY_TIME); // This is used for timer calculations!  Keep this util::DELAY_TIME
    }
}
