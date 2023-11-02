#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"

// Define the trigger and echo pins
#define TRIGGER_PIN 19
#define ECHO_PIN 18
#define SPEED_OF_SOUND_CM_US 0.0343
#define MAX_WAIT_TIME_US 10000
#define OBJECT_DETECTION_THRESHOLD 6.0
#define NUM_AVERAGES 8

// Global variables for object detection
volatile bool object_detected = false;
volatile float latest_distance_cm = 0.0;

// Callback function for the echo interrupt
void echo_interrupt_handler() {
    // Check if the echo pin is high (object detected) and distance is below the threshold
    if (gpio_get(ECHO_PIN) && latest_distance_cm <= OBJECT_DETECTION_THRESHOLD) {
        object_detected = true;
    }
}

// Measure distance using the ultrasonic sensor
float measure_distance() {
    // Trigger the sensor by setting the trigger pin high and low
    gpio_put(TRIGGER_PIN, 1);
    sleep_us(10);
    gpio_put(TRIGGER_PIN, 0);

    // Measure the time it takes for the echo pulse to return
    uint32_t start_time = time_us_32();
    while (gpio_get(ECHO_PIN) == 0 && time_us_32() - start_time < MAX_WAIT_TIME_US);
    start_time = time_us_32();

    while (gpio_get(ECHO_PIN) == 1 && time_us_32() - start_time < MAX_WAIT_TIME_US);
    uint32_t end_time = time_us_32();

    // Calculate the distance from the duration of the echo pulse
    uint32_t duration = end_time - start_time;
    float distance = (float)duration * SPEED_OF_SOUND_CM_US / 2.0;
    return distance;
}

// Calculate the average of an array of distances
float calculate_average(float distances[], int size) {
    float total = 0.0;
    for (int i = 0; i < size; i++) {
        total += distances[i];
    }
    return total / size;
}

int main() {
    stdio_init_all();

    // Initialize the trigger and echo pins
    gpio_init(TRIGGER_PIN);
    gpio_set_dir(TRIGGER_PIN, GPIO_OUT);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    // Enable an interrupt on the echo pin for rising edge events
    gpio_set_irq_enabled_with_callback(ECHO_PIN, GPIO_IRQ_EDGE_RISE, true, echo_interrupt_handler);

    float raw_distances[NUM_AVERAGES];
    int average_counter = 0;

    while (1) {
        // Collect multiple distance measurements
        for (int i = 0; i < NUM_AVERAGES; i++) {
            float distance = measure_distance();
            raw_distances[i] = distance;
        }

        // Calculate the average distance from the measurements
        float average_distance = calculate_average(raw_distances, NUM_AVERAGES);
        latest_distance_cm = average_distance;

        // Check if an object is detected based on the threshold
        if (latest_distance_cm <= OBJECT_DETECTION_THRESHOLD) {
            object_detected = true;
            printf("Object detected\n");
            printf("Distance: %d cm\n", (int)latest_distance_cm);
        } else {
            object_detected = false;
        }

        // Delay before the next measurement
        sleep_ms(1);

        average_counter = (average_counter + 1) % NUM_AVERAGES;
    }

    return 0;
}
