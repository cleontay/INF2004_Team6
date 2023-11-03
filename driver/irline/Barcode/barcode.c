#include "barcode.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

// Check for high pins
bool isHigh = false;

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
    printf("%s\n", detector->pattern_buffer);
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
    if (detector->barcode_started == 0 && sensor_data>=detector->line_threshold) {
        startBarcode(detector);
    }
    else if (sensor_data>=detector->line_threshold){
        if (detector->low_duration > 0) {
            finishLowBarcode(detector);
        }
        else {
            addToHighBarcode(detector);
        }
    }
}

// Manage low state
void isLowState(BarcodeDetector *detector) {
    uint16_t sensor_data = adc_read();
    if(sensor_data < detector->line_threshold) {
        if (detector->barcode_started == 1) {
        if (detector->high_duration > 0) {
            finishHighBarcode(detector);
        }
        else {
            if (detector->low_duration > 4) {
                endBarcode(detector);
            }
            else {
                addToLowBarcode(detector);
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


