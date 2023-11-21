#include <stdio.h>
#include "ultrasonic.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"

#define TRIGGER_PIN 19
#define ECHO_PIN 18
#define SPEED_OF_SOUND_CM_US 0.0343
#define MAX_WAIT_TIME_US 10000
#define OBJECT_DETECTION_THRESHOLD 6.0
#define NUM_AVERAGES 8

volatile bool object_detected = false;
volatile float latest_distance_cm = 0.0;

void echo_interrupt_handler() {
    if (gpio_get(ECHO_PIN) && latest_distance_cm <= OBJECT_DETECTION_THRESHOLD) {
        object_detected = true;
    }
}

float measure_distance() {
    gpio_put(TRIGGER_PIN, 1);
    sleep_us(10);
    gpio_put(TRIGGER_PIN, 0);

    uint32_t start_time = time_us_32();
    while (gpio_get(ECHO_PIN) == 0 && time_us_32() - start_time < MAX_WAIT_TIME_US);
    start_time = time_us_32();

    while (gpio_get(ECHO_PIN) == 1 && time_us_32() - start_time < MAX_WAIT_TIME_US);
    uint32_t end_time = time_us_32();

    uint32_t duration = end_time - start_time;
    float distance = (float)duration * SPEED_OF_SOUND_CM_US / 2.0;
    return distance;
}

float calculate_average(float distances[], int size) {
    float total = 0.0;
    for (int i = 0; i < size; i++) {
        total += distances[i];
    }
    return total / size;
}

void perform_distance_measurement(float raw_distances[], int* average_counter) {
    for (int i = 0; i < NUM_AVERAGES; i++) {
        float distance = measure_distance();
        raw_distances[i] = distance;
    }
    *average_counter = (*average_counter + 1) % NUM_AVERAGES;
}

void check_and_print_object_detection() {
    float raw_distances[NUM_AVERAGES];
    int average_counter = 0;

    while (1) {
        perform_distance_measurement(raw_distances, &average_counter);

        float average_distance = calculate_average(raw_distances, NUM_AVERAGES);
        latest_distance_cm = average_distance;

        if (latest_distance_cm <= OBJECT_DETECTION_THRESHOLD) {
            object_detected = true;
            printf("Object detected\n");
            printf("Distance: %d cm\n", (int)latest_distance_cm);
        } else {
            object_detected = false;
        }

        sleep_ms(1);
    }
}
