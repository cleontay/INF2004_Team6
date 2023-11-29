/**
 * @file main.c
 * @brief This file contains the main function for the barcode scanner driver.
 *
 * The main function initializes the necessary GPIO pins, sets up the interrupt handler,
 * and checks for current edge state of the pin, and determine what to do during these states.
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "barcode2.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define IR_SENSOR_PIN 28

int main(void) {
    // Initialise pins, adc and barcode detector
    stdio_init_all();
    adc_init();
    adc_set_temp_sensor_enabled(true);
    BarcodeDetector barcodeDetector;
    initBarcodeDetector(&barcodeDetector);

    // Initialize the GPIO pin for the IR sensor as an input
    gpio_init(IR_SENSOR_PIN);
    gpio_set_dir(IR_SENSOR_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(IR_SENSOR_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &IR_sensor_handler);

    while(1) {
        // Manage pin states
        if (isHigh == true) {
            isHighState(&barcodeDetector);
        }
        else {
            isLowState(&barcodeDetector);
        }
        
        sleep_ms(200);
    }
}