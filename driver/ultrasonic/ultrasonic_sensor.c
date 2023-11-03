#include <stdio.h>
#include <stdlib.h>
#include "ultrasonic_sensor.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"

#define TRIGGER_PIN 19
#define ECHO_PIN 18
#define SPEED_OF_SOUND_CM_US 0.0343
#define MAX_WAIT_TIME_US 10000
#define OBJECT_DETECTION_THRESHOLD 6.0
#define NUM_AVERAGES 8

static bool object_detected = false;
static float latest_distance_cm = 0.0;

static void echo_interrupt_handler() {
    if (gpio_get(ECHO_PIN) && latest_distance_cm <= OBJECT_DETECTION_THRESHOLD) {
        object_detected = true;
    }
}

void ultrasonic_sensor_init() {
    stdio_init_all();
    gpio_init(TRIGGER_PIN);
    gpio_set_dir(TRIGGER_PIN, GPIO_OUT);
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(ECHO_PIN, GPIO_IRQ_EDGE_RISE, true, echo_interrupt_handler);
}

float ultrasonic_sensor_measure_distance() {
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

bool ultrasonic_sensor_is_object_detected() {
    return object_detected;
}
