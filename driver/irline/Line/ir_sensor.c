#include <stdio.h>
#include "ir_sensor.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

void ir_sensor_init(unsigned int sensor_pin) {
    gpio_init(sensor_pin);
    gpio_set_dir(sensor_pin, GPIO_IN);
}

bool ir_sensor_is_line_detected(unsigned int sensor_pin) {
    return gpio_get(sensor_pin);
}

/* 
Example Usage:
#include <stdio.h>
#include "pico/stdlib.h"
#include "ir_sensor.h"

#define IR_SENSOR1_PIN 26
#define IR_SENSOR2_PIN 27

int main() {
    stdio_init_all();

    ir_sensor_init(IR_SENSOR1_PIN);
    ir_sensor_init(IR_SENSOR2_PIN);

    while (1) {
        bool lineDetected1 = ir_sensor_is_line_detected(IR_SENSOR1_PIN);
        bool lineDetected2 = ir_sensor_is_line_detected(IR_SENSOR2_PIN);

        if (lineDetected1) {
            printf("Line detected by IR Sensor 1\n");
        }

        if (lineDetected2) {
            printf("Line detected by IR Sensor 2\n");
        }

        // Implement your logic here to control the car based on which sensor detected the line
        // You can use if-else or a state machine for more complex behavior

        sleep_ms(100);
    }

    return 0;
}

 */