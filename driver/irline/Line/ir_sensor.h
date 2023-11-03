// ir_sensor.h

#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include <stdbool.h>

// Initialize the IR sensor
void ir_sensor_init(unsigned int sensor_pin);

// Check if a line is detected by the IR sensor
bool ir_sensor_is_line_detected(unsigned int sensor_pin);

#endif
