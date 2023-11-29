#ifndef IR_SENSOR_H
#define IR_SENSOR_H

// Enumeration types for motor
enum MotorState {
    NO_LINE,
    LEFT_LINE,
    RIGHT_LINE,
    BOTH_LINES
};

// Callback for motor state changes
typedef void (*MotorStateCallback)(enum MotorState state);

void init_sensors();
enum MotorState determine_motor_state();

#endif 

// Initialise right and left IR sensors
void init_sensors() {
    gpio_init(IR_SENSOR1_PIN);
    gpio_init(IR_SENSOR2_PIN);
}

/**
 * @brief Determines the state of the motor based on the IR sensor readings.
 * 
 * This function reads the pin edge state of the IR sensors and determines the state of the motor.
 * 
 * @return The state of the motor (NO_LINE, LEFT_LINE, RIGHT_LINE, or BOTH_LINES).
 */
enum MotorState determine_motor_state() {
    
    // If both high, interrupt and stop
    if (gpio_get(IR_SENSOR1_PIN) == 1 && gpio_get(IR_SENSOR2_PIN) == 1) {
        printf("Both line detected (Interrupt)\n");
        return BOTH_LINES;
    }

    // If right sensor is high, return right line
    else if (gpio_get(IR_SENSOR1_PIN) == 1) {
        return RIGHT_LINE;

    // If left sensor is high, return left line
    } else if (gpio_get(IR_SENSOR2_PIN) == 1) {
        return LEFT_LINE;

    // Otherwise, continue.
    } else 
    return NO_LINE;
}