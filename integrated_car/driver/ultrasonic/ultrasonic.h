#ifndef ULTRASONIC_H
#define ULTRASONIC_H

/**
 * @brief Header file for the Ultrasonic module.
 * 
 * This module provides functions to interface with an ultrasonic sensor
 * for measuring distances. It includes functions for initializing the sensor,
 * triggering a distance measurement, and handling interrupt events.
 */

volatile float latest_distance_cm; /**< Variable to store the latest measured distance in centimeters */
uint32_t start_time, end_time; /**< Variables to store the start and end times of the echo pulse */


void echo_interrupt_handler(uint gpio, uint32_t events);

void irq_callback(uint gpio, uint32_t events);
void ultrasonic_init();


void trigger_pulse();

#endif // ULTRASONIC_H

/**
 * @brief Interrupt handler for the echo pin.
 * 
 * This function is called when an interrupt event is triggered on the echo pin.
 * It calculates the duration of the echo pulse and converts it to a distance value.
 * If the distance is below a certain threshold, it sets the object_detected flag to true.
 * 
 * @param gpio The GPIO pin number that triggered the interrupt
 * @param events The interrupt events that occurred
 */
void echo_interrupt_handler(uint gpio, uint32_t events) {
    if(events & GPIO_IRQ_EDGE_RISE){
        start_time = time_us_32();
    }
    else if (events & GPIO_IRQ_EDGE_FALL){
        end_time = time_us_32();
        uint32_t duration = end_time - start_time;
        float distance = (float)duration * SPEED_OF_SOUND_CM_US / 2.0;
        if(distance <= OBJECT_DETECTION_THRESHOLD){
            object_detected = true;
        }
        else{
            object_detected = false;
        }
    }
}

/**
 * @brief Initialize the ultrasonic sensor module.
 * 
 * This function initializes the GPIO pins used for the ultrasonic sensor.
 * It sets the trigger pin as an output and the echo pin as an input.
 * It also registers the irq_callback function as the interrupt callback for the echo pin.
 */
void ultrasonic_init(){
    gpio_init(TRIGGER_PIN);
    gpio_set_dir(TRIGGER_PIN, GPIO_OUT);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    gpio_set_irq_enabled_with_callback(ECHO_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &irq_callback);
}

/**
 * @brief Trigger a pulse on the ultrasonic sensor.
 * 
 * This function sends a trigger pulse to the ultrasonic sensor.
 * It sets the trigger pin high for a short duration and then sets it low.
 */
void trigger_pulse(){
    gpio_put(TRIGGER_PIN, 1);
    sleep_us(10);
    gpio_put(TRIGGER_PIN, 0);
}