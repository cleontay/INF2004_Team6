#ifndef ULTRASONIC_H
#define ULTRASONIC_H

volatile float latest_distance_cm;

void echo_interrupt_handler();
float measure_distance();
float calculate_average(float distances[], int size);
void perform_distance_measurement(float raw_distances[], int* average_counter);
void check_and_print_object_detection();

#endif // ULTRASONIC_H

void echo_interrupt_handler() {
    if (gpio_get(ECHO_PIN) && latest_distance_cm <= OBJECT_DETECTION_THRESHOLD) {
        object_detected = true;
    }
}

float measure_distance() {
    gpio_put(TRIGGER_PIN, 1);
    sleep_us(10);
    gpio_put(TRIGGER_PIN, 0);
    
    printf("Trigger pulse sent\n");

    uint32_t start_time = time_us_32();
    while (gpio_get(ECHO_PIN) == 0 && time_us_32() - start_time < MAX_WAIT_TIME_US);
    start_time = time_us_32();

    while (gpio_get(ECHO_PIN) == 1 && time_us_32() - start_time < MAX_WAIT_TIME_US);
    uint32_t end_time = time_us_32();

    uint32_t duration = end_time - start_time;
    printf("Raw Duration: %lu us\n", duration);
    float distance = (float)duration * SPEED_OF_SOUND_CM_US / 2.0;
    printf("Distance: %.2f cm\n", distance);
    return distance;
}

float calculate_average(float distances[], int size) {
    float total = 0.0;
    for (int i = 0; i < size; i++) {
        total += distances[i];
    }
    return total / size;
}

// void echo_interrupt_handler() {
//     if (gpio_get(ECHO_PIN) && latest_distance_cm <= OBJECT_DETECTION_THRESHOLD) {
//         object_detected = true;
//     }
// }

// float measure_distance() {
//     gpio_put(TRIGGER_PIN, 1);
//     sleep_us(10);
//     gpio_put(TRIGGER_PIN, 0);

//     printf("Trigger pulse sent\n");

//     uint32_t start_time = time_us_32();
//     while (gpio_get(ECHO_PIN) == 0 && time_us_32() - start_time < MAX_WAIT_TIME_US);
//     if(time_us_32() - start_time >= MAX_WAIT_TIME_US) {
//         printf("Timeout waiting for echo pin to go high\n");
//         return -1;
//     }
//     start_time = time_us_32();

//     while (gpio_get(ECHO_PIN) == 1 && time_us_32() - start_time < MAX_WAIT_TIME_US);
//     if(time_us_32() - start_time >= MAX_WAIT_TIME_US) {
//         printf("Timeout waiting for echo pin to go low\n");
//         return -1;
//     }
//     uint32_t end_time = time_us_32();

//     uint32_t duration = end_time - start_time;
//     float distance = (float)duration * SPEED_OF_SOUND_CM_US / 2.0;

//     printf("Duration: %lu us, Distance: %.2f cm\n", duration, distance);

//     return distance;
// }

// float calculate_average(float distances[], int size) {
//     if(size == 0) {
//         printf("Error: size is zero\n");
//         return -1;
//     }
//     float total = 0.0;
//     for (int i = 0; i < size; i++) {
//         total += distances[i];
//     }
//     return total / size;
// }

// void perform_distance_measurement(float raw_distances[], int* average_counter) {
//     for (int i = 0; i < NUM_AVERAGES; i++) {
//         float distance = measure_distance();
//         if(distance == -1) {
//             printf("Error measuring distance\n");
//             return;
//         }
//         raw_distances[i] = distance;
//     }
//     *average_counter = (*average_counter + 1) % NUM_AVERAGES;
// }

// void check_and_print_object_detection() {
//     float raw_distances[NUM_AVERAGES];
//     int average_counter = 0;

//     while (1) {
//         perform_distance_measurement(raw_distances, &average_counter);

//         float average_distance = calculate_average(raw_distances, NUM_AVERAGES);
//         if(average_distance == -1) {
//             printf("Error calculating average distance\n");
//             continue;
//         }
//         latest_distance_cm = average_distance;

//         if (latest_distance_cm <= OBJECT_DETECTION_THRESHOLD) {
//             object_detected = true;
//             printf("Object detected\n");
//             printf("Distance: %d cm\n", (int)latest_distance_cm);
//         } else {
//             object_detected = false;
//         }

//         sleep_ms(200);
//     }
// }
