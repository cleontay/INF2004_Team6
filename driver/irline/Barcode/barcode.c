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
    decodeBarcode(detector);
    printf("%s\n", detector->pattern_buffer);
    memset(detector->pattern_buffer, 0, sizeof(detector->pattern_buffer));
    detector->pattern_index = 0;
}

void decodeBarcode(BarcodeDetector *detector) {
    int group_size = 9;
    char test[] = "tStsTsTstStSTsTststStStsTsTst";
    char ast[] = "tStsTsTst", z[] = "tSTsTstst", a[] = "TststStsT", f[] = "tsTsTStst";
    char new_pattern_buffer[3];
    int new_pattern_length = 0;

    for (int i = 0; i < strlen(test); i += group_size+1) {

        char current_group[10]; // 9 characters + null terminator
        strncpy(current_group, test + i, group_size);
        current_group[group_size] = '\0'; // Null-terminate the substring

        printf("%s\n", current_group);

        // Check if the current group matches patterns
        if (strcmp(current_group, ast) == 0) {
            new_pattern_buffer[new_pattern_length] = '*';
            new_pattern_length++;
        }
        else if (strcmp(current_group, z) == 0) {
            new_pattern_buffer[new_pattern_length] = 'Z';
            new_pattern_length++;
        }
        else if (strcmp(current_group, a) == 0) {
            new_pattern_buffer[new_pattern_length] = 'A';
            new_pattern_length++;
        }
        memset(current_group, 0, sizeof(current_group));
    }
    if (new_pattern_buffer[0] == '*' && new_pattern_buffer[2] == '*') {
        if (new_pattern_length>0) {
        new_pattern_buffer[3] = '\0';
        printf("Barcode detected and decoded into: %s\n", new_pattern_buffer);
        }
    }
    else {
        printf("Misshapen barcode");
    }

    
    
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



