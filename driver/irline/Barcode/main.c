#include <stdio.h>
#include "pico/stdlib.h"
#include "barcode.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define IR_SENSOR_PIN 26

int main(void) {
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
        // Manage states
        if (isHigh == true) {
            isHighState(&barcodeDetector);
        }
        else {
            isLowState(&barcodeDetector);
        }
        
        sleep_ms(200);
    }
}
