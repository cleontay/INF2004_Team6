#include <stdio.h>
#include "irsensor.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define IR_SENSOR1_PIN 26 // assume left
#define IR_SENSOR2_PIN 27 // assume right
#define INTERRUPT_PIN 14

void adc_init_sensors() {
    adc_init();
    adc_gpio_init(IR_SENSOR1_PIN);
    adc_gpio_init(IR_SENSOR2_PIN);
}

enum MotorState determine_motor_state() {
    adc_select_input(0); // select channel 0 for pin 26
    uint16_t left_ir_sensor = adc_read();

    adc_select_input(1); // select channel 1 for pin 27
    uint16_t right_ir_sensor = adc_read();

    if (left_ir_sensor > 2000 && right_ir_sensor < 2000) {
        return RIGHT_LINE;
    } else if (left_ir_sensor < 2000 && right_ir_sensor > 2000) {
        return LEFT_LINE;
    } else if (left_ir_sensor < 2000 && right_ir_sensor < 2000) {
        return BOTH_LINES;
    } else if (left_ir_sensor > 2000 && right_ir_sensor > 2000) {
        return NO_LINE;
    }

    return NO_LINE;
}

void gpio_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_RISE) {
        enum MotorState current_state = determine_motor_state();
        printf("Interrupt motor\n");
        interrupt_handle_motor_state_change(current_state);
    }
}

void interrupt_handle_motor_state_change(enum MotorState state) {
    switch (state) {
        case NO_LINE:
            break;
        case LEFT_LINE:
            printf("Line detected on the left (Interrupt)\n");
            break;
        case RIGHT_LINE:
            printf("Line detected on the right (Interrupt)\n");
            break;
        case BOTH_LINES:
            printf("Both lines detected (Interrupt)\n");
            break;
    }
    gpio_put(INTERRUPT_PIN, 0);
}

void handle_motor_state_change(enum MotorState state) {
    // modify this to change what the motor should do
    switch (state) {
        case NO_LINE:
            printf("No line detected\n");
            break;
        case LEFT_LINE:
            printf("Line detected on the left\n");
            gpio_put(INTERRUPT_PIN, 1);
            break;
        case RIGHT_LINE:
            printf("Line detected on the right\n");
            gpio_put(INTERRUPT_PIN, 1);
            break;
        case BOTH_LINES:
            printf("Both lines detected\n");
            gpio_put(INTERRUPT_PIN, 1);
            break;
    }
}
