// ir_sensor.h

#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include <stdbool.h>

// IR sensor struct to represent each sensor
typedef struct {
    unsigned int pin;
    bool detected; // Flag to store detection status
} IR_Sensor;

// Initialize an IR sensor
void ir_sensor_init(IR_Sensor* sensor, unsigned int pin);

// Check if a line is detected by the IR sensor
bool ir_sensor_detected(const IR_Sensor* sensor);

// Handler for the IR sensor interrupt
void ir_sensor_interrupt_handler(IR_Sensor* sensor);

#endif