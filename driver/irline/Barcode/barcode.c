#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define N_SAMPLES 1000
uint16_t sample_buf[N_SAMPLES];
#define IR_SENSOR_PIN 26

// Declaring barcode variables
typedef struct {
    int high_duration;
    int low_duration;
    int barcode_started;
    char pattern_buffer[30];
    int pattern_index;
    int line_threshold;
} BarcodeDetector;

// Check for high pins
bool isHigh = false;

// Global barcode detector
BarcodeDetector barcodeDetector;

// Initialise the variables
void initBarcodeDetector(BarcodeDetector *detector) {
    // Initialize detector settings
    detector->high_duration = 0;
    detector->low_duration = 0;
    detector->barcode_started = 0;
    detector->pattern_index = 0;
    detector->line_threshold = 1000;
}

// Declare start of barcode
void startBarcode(BarcodeDetector *detector) {
    detector->high_duration++;
    detector->barcode_started = 1;

}

// End the barcode here (Can be fixed to return a value). Variables are reset
void endBarcode(BarcodeDetector *detector) {
    detector->pattern_buffer[detector->pattern_index] = '\0';
    detector->barcode_started = 0;
    detector->low_duration = 0;
    printf("%s\n", barcodeDetector.pattern_buffer);
    memset(detector->pattern_buffer, 0, sizeof(detector->pattern_buffer));
    detector->pattern_index = 0;
}

void finishLowBarcode(BarcodeDetector *detector) {
    // If end sequence detected, end scan
    if (detector->low_duration > 4 && detector->pattern_index > 4) {
        endBarcode(detector);
    }
    // If barcode is low for long enough, it's a big space
    else if (detector->low_duration > 2) {
        detector->pattern_buffer[detector->pattern_index] = 'S';
        detector->pattern_index++;

    // If barcode is low for a small amount of time, it is small space
    } else if (detector->low_duration > 0) {
        detector->pattern_buffer[detector->pattern_index] = 's';
        detector->pattern_index++;
    }
    detector->high_duration++; // Increment the 'high' duration
    detector->low_duration = 0; // Reset the 'low' duration

}

void finishHighBarcode(BarcodeDetector *detector) {

    // If the barcode is high for long enough, it's a big line
    if (detector->high_duration > 2) {
        detector->pattern_buffer[detector->pattern_index] = 'T';
        detector->pattern_index++;

    // If the barcode is high for a small amount of time, it's a small line
    } else if (detector->high_duration > 0) {
        detector->pattern_buffer[detector->pattern_index] = 't';
        detector->pattern_index++;
    }
    detector->low_duration++; // Increment the 'low' duration
    detector->high_duration = 0; // Reset the 'high' duration
}

// Increment high pulse counter
void addToHighBarcode(BarcodeDetector *detector) {
    detector->high_duration++;
}

// Increment low pulse counter
void addToLowBarcode(BarcodeDetector *detector) {
    detector->low_duration++;
    detector->high_duration = 0;
}

// Manage high state
void isHighState(BarcodeDetector *detector) {
    uint16_t sensor_data = adc_read();
    if (barcodeDetector.barcode_started == 0 && sensor_data>=barcodeDetector.line_threshold) {
        startBarcode(&barcodeDetector);
    }
    else if (sensor_data>=barcodeDetector.line_threshold){
        if (barcodeDetector.low_duration > 0) {
            finishLowBarcode(&barcodeDetector);
        }
        else {
            addToHighBarcode(&barcodeDetector);
        }
    }
}

// Manage low state
void isLowState(BarcodeDetector *detector) {
    uint16_t sensor_data = adc_read();
    if(sensor_data < barcodeDetector.line_threshold) {
        if (barcodeDetector.barcode_started == 1) {
        if (barcodeDetector.high_duration > 0) {
            finishHighBarcode(&barcodeDetector);
        }
        else {
            if (barcodeDetector.low_duration > 4) {
                endBarcode(&barcodeDetector);
            }
            else {
                addToLowBarcode(&barcodeDetector);
            }
        }
    }
    }
    
}

// Handle and set states
void IR_sensor_handler(uint gpio, uint32_t events) {

    if (events & GPIO_IRQ_EDGE_RISE) {
        isHigh = true;
    }
    else if (events & GPIO_IRQ_EDGE_FALL) {
        isHigh = false;
    }
}


int main(void) {
    stdio_init_all();
    adc_init();
    adc_set_temp_sensor_enabled(true);
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