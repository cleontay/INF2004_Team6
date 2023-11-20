#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stdbool.h>

void echo_interrupt_handler();
float measure_distance();
float calculate_average(float distances[], int size);
void perform_distance_measurement(float raw_distances[], int* average_counter);
void check_and_print_object_detection();

extern volatile bool object_detected;
extern volatile float latest_distance_cm;

#endif // ULTRASONIC_H
