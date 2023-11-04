// ir_sensor.c
#include <stdio.h>
#include "ir_sensor.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Callback function for IR sensor interrupt
static void ir_sensor_callback(uint gpio, uint32_t events) {
    IR_Sensor* sensor = (IR_Sensor*)gpio_get_irq_user_data(gpio);
    sensor->detected = true;
}

void ir_sensor_init(IR_Sensor* sensor, unsigned int pin) {
    sensor->pin = pin;
    sensor->detected = false;
    
    gpio_init(sensor->pin);
    gpio_set_dir(sensor->pin, GPIO_IN);
    
    // Configure an interrupt for the IR sensor
    gpio_set_irq_enabled_with_callback(sensor->pin, GPIO_IRQ_EDGE_RISE, true, ir_sensor_callback);
    gpio_set_irq_enabled(sensor->pin, true);
    gpio_set_irq_handler(sensor->pin, ir_sensor_callback);
    gpio_set_irq_user_data(sensor->pin, sensor);
}

bool ir_sensor_detected(const IR_Sensor* sensor) {
    return sensor->detected;
}

// Callback function to handle IR sensor detection
void ir_sensor_interrupt_handler(IR_Sensor* left_sensor, IR_Sensor* right_sensor) {
    bool left_detected = ir_sensor_detected(left_sensor);
    bool right_detected = ir_sensor_detected(right_sensor);

    if (left_detected && !right_detected) {
        // If the left sensor detects a line and the right sensor doesn't, turn right
        // Implement code to turn right
        printf("Turn right\n");
    } else if (!left_detected && right_detected) {
        // If the right sensor detects a line and the left sensor doesn't, turn left
        // Implement code to turn left
        printf("Turn left\n");
    } else if (!left_detected && !right_detected) {
        // If both sensors do not detect anything, stop
        // Implement code to stop
        printf("Stop\n");
    }
}

