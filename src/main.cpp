#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
pros::MotorGroup leftMotors({-5, 4, -3},
                            pros::MotorGearset::blue); // left motor group - ports 3 (reversed), 4, 5 (reversed)
pros::MotorGroup rightMotors({6, -9, 7}, pros::MotorGearset::blue); // right motor group - ports 6, 7, 9 (reversed)

// Inertial Sensor on port 10
pros::Imu imu(10);

// tracking wheels
// horizontal tracking wheel encoder. Rotation sensor, port 20, not reversed
pros::Rotation horizontalEnc(20);
// vertical tracking wheel encoder. Rotation sensor, port 11, reversed
pros::Rotation verticalEnc(-11);
// horizontal tracking wheel. 2.75" diameter, 5.75" offset, back of the robot (negative)
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_275, -5.75);
// vertical tracking wheel. 2.75" diameter, 2.5" offset, left of the robot (negative)
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_275, -2.5);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              10, // 10 inch track width
                              lemlib::Omniwheel::NEW_4, // using new 4" omnis
                              360, // drivetrain rpm is 360
                              2 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
lemlib::ControllerSettings linearController(10, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            3, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            20 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(2, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             10, // derivative gain (kD)
                                             3, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

// sensors for odometry
lemlib::OdomSensors sensors(&vertical, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            &horizontal, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

// Additional sensors
pros::Distance distanceSensor(12); // Distance sensor on port 12
pros::Optical opticalSensor(13);   // Optical sensor on port 13
pros::Vision visionSensor(14);     // Vision sensor on port 14

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors

    // Ensure additional sensors are working
    if (!imu.is_calibrating()) {
        pros::lcd::print(3, "Inertial sensor calibrated.");
    } else {
        pros::lcd::print(3, "Calibrating inertial sensor...");
    }

    pros::lcd::print(4, "Distance Sensor: %d mm", distanceSensor.get());
    pros::lcd::print(5, "Optical Sensor Hue: %.2f", opticalSensor.get_hue());
    pros::lcd::print(6, "Vision Sensor Object Count: %d", visionSensor.get_object_count());
}

/**
 * Runs while the robot is disabled
 */
void disabled() {}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}

/**
 * Runs during auto
 */
void autonomous() {
    pros::lcd::print(0, "Starting Autonomous");

    // Loop to find and collect multiple red rings
    for (int ringCount = 0; ringCount < 3; ++ringCount) {
        pros::lcd::print(1, "Searching for red ring #%d", ringCount + 1);

        // Vision sensor: Locate red object
        auto redObject = visionSensor.get_by_sig(1); 
        if (redObject.signature == 1) {
            pros::lcd::print(2, "Red object found at X: %d, Y: %d", redObject.x_middle_coord, redObject.y_middle_coord);

            // Align the robot to the red object using vision data
            chassis.turnToPoint(redObject.x_middle_coord, redObject.y_middle_coord, 1000);

            // Distance sensor: Approach the object
            while (distanceSensor.get() > 150) { 
                chassis.arcade(50, 0); 
                pros::lcd::print(3, "Distance: %d mm", distanceSensor.get());
                pros::delay(50);
            }
            chassis.stop(); 

            // Optical sensor: Verify the object is red
            auto hue = opticalSensor.get_hue();
            if (hue > 0 && hue < 60) { 
                pros::lcd::print(4, "Red object confirmed by optical sensor (hue: %.2f)", hue);

                intake.spin_forward();
                pros::delay(1000); 
                intake.stop();
            } else {
                pros::lcd::print(4, "Color mismatch: Detected hue %.2f", hue);
            }
        } else {
            pros::lcd::print(2, "No red object found. Searching...");
            chassis.turnToHeading(chassis.getPose().theta + 30, 1000); 
        }
    }

    // Move to a new location after collecting all rings
    pros::lcd::print(5, "All rings collected. Moving forward and left.");
    chassis.moveToPose(24, 0, 0, 2000); 
    chassis.turnToHeading(90, 1000); 
    chass



/**
 * Runs in driver control
 */
void opcontrol() {
    while (true) {
        // Drive the chassis with curvature drive
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        chassis.arcade(leftY, rightX);


        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            clamp.extend();
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            clamp.retract();
        }

        pros::delay(10);

        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            intake.spin_forward(); 
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            intake.spin_reverse();
        } else {
            intake.stop();
        }

        pros::delay(10);

        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            lift.move_up(); // Move lift up
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            lift.move_down(); // Move lift down
        } else {
            lift.stop(); // Stop lift
        }

        pros::delay(10);

        // Continuously display sensor data
        pros::lcd::print(0, "Distance: %d mm", distanceSensor.get());
        pros::lcd::print(1, "Optical Hue: %.2f", opticalSensor.get_hue());
        pros::lcd::print(2, "Proximity: %d", opticalSensor.get_proximity());
        pros::lcd::print(3, "Inertial Heading: %.2f", imu.get_heading());
        pros::lcd::print(4, "Vision Objects: %d", visionSensor.get_object_count());

        pros::delay(100);
    }
}
