#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define IR_SENSOR1_PIN 26
#define IR_SENSOR2_PIN 27

// Function to check if an IR sensor detects a line
bool isLineDetected(uint gpio_pin) {
    return gpio_get(gpio_pin);
}

// Function to handle the car's behavior based on sensor readings
void handleCarBehavior() {
    if (isLineDetected(IR_SENSOR1_PIN)) {
        printf("Line detected by IR Sensor 1\n");
        // Implement behavior for Sensor 1 detection
    }

    if (isLineDetected(IR_SENSOR2_PIN)) {
        printf("Line detected by IR Sensor 2\n");
        // Implement behavior for Sensor 2 detection
    }

    // Implement complex behavior here later
    // using if-else or a state machine for more complex behavior
}

int main() {
    stdio_init_all();
    
    gpio_init(IR_SENSOR1_PIN);
    gpio_set_dir(IR_SENSOR1_PIN, GPIO_IN);

    gpio_init(IR_SENSOR2_PIN);
    gpio_set_dir(IR_SENSOR2_PIN, GPIO_IN);

    while (1) {
        handleCarBehavior();

        sleep_ms(100);
    }

    return 0;
}
