#ifndef IR_SENSOR_H
#define IR_SENSOR_H

enum MotorState {
    NO_LINE,
    LEFT_LINE,
    RIGHT_LINE,
    BOTH_LINES
};

typedef void (*MotorStateCallback)(enum MotorState state);

void init_sensors();
enum MotorState determine_motor_state();

#endif 

void init_sensors() {
    gpio_init(IR_SENSOR1_PIN);
    gpio_init(IR_SENSOR2_PIN);
}

enum MotorState determine_motor_state() {
    printf("%i,%i\n",gpio_get(IR_SENSOR1_PIN),gpio_get(IR_SENSOR2_PIN));
    if (gpio_get(IR_SENSOR1_PIN) == 1 && gpio_get(IR_SENSOR2_PIN) == 1) {
        // printf("Both line detected (Interrupt)\n");
        return BOTH_LINES;
    }
    else if (gpio_get(IR_SENSOR1_PIN) == 1) {
        //printf("Line detected on the right (Interrupt)\n");
        return RIGHT_LINE;
    } else if (gpio_get(IR_SENSOR2_PIN) == 1) {
        //printf("Line detected on the left (Interrupt)\n");
        return LEFT_LINE;
    } else 
    return NO_LINE;
}
