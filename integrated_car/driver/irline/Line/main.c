/**
 * @file main.c
 * @brief This file contains the outdated main function for the irsensor driver.
 *
 * The main function initializes the necessary GPIO pins, sets up the interrupt handler,
 * and checks for current sensor detection readings for the motor state.
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "irsensor.h"
#include "hardware/gpio.h"

#define INTERRUPT_PIN 14

int main() {
    // Initialise GPIO pins, adc and interrupt handler
    stdio_init_all();
    adc_init_sensors();
    gpio_init(INTERRUPT_PIN);
    gpio_set_dir(INTERRUPT_PIN, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(INTERRUPT_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Enable callback of motor state
    MotorStateCallback motor_state_callback = handle_motor_state_change;

    while (1) {
        // Retrieve and handle motor state
        motor_state_callback(determine_motor_state());
        sleep_ms(100);
    }

    return 0;
}
