#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include <stdint.h>

enum MotorState {
    NO_LINE,
    LEFT_LINE,
    RIGHT_LINE,
    BOTH_LINES
};

typedef void (*MotorStateCallback)(enum MotorState state);

void adc_init_sensors();
enum MotorState determine_motor_state();
void interrupt_handle_motor_state_change(enum MotorState state);
void handle_motor_state_change(enum MotorState state);
void gpio_callback(uint gpio, uint32_t events);


#endif // IR_SENSOR_H