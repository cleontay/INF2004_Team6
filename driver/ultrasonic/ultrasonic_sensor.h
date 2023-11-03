// ultrasonic_sensor.h

#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <stdbool.h>

// Initialize the ultrasonic sensor
void ultrasonic_sensor_init();

// Measure the distance using the ultrasonic sensor
float ultrasonic_sensor_measure_distance();

// Check if an object is detected
bool ultrasonic_sensor_is_object_detected();

#endif