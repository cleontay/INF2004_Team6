#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Define the GPIO pin connected to the IR line sensor
#define IR_SENSOR_PIN 26

int main() {
    stdio_init_all(); // Initialize serial communication for debugging

    // Initialize the GPIO pin for the IR sensor as an input
    gpio_init(IR_SENSOR_PIN);
    gpio_set_dir(IR_SENSOR_PIN, GPIO_IN);

    while (1) {
        // Read the value of the IR sensor
        bool lineDetected = gpio_get(IR_SENSOR_PIN);

        if (lineDetected) {
            printf("Detected a black line!\n");
        }

        sleep_ms(100); // Adjust the sleep time as needed
    }

    return 0;
}
