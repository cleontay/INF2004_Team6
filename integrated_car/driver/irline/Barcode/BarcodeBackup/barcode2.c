#include "barcode2.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

bool isHigh = false;

// Initialise the variables
void initBarcodeDetector(BarcodeDetector *detector) {
    // Initialize detector settings
    detector->high_duration = 0;
    detector->low_duration = 0;
    detector->barcode_started = 0;
    detector->pattern_index = 0;
    detector->line_threshold = 2000;
}

// Declare start of barcode and increment
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
    // Check for incomplete barcode
    if(strlen(detector->pattern_buffer) == 29) {
        decodeBarcode(detector);
    }
    else if(strlen(detector->pattern_buffer)>1) {
        printf("Misshapen barcode: Length\n");
    }
    
    // Reset buffers
    memset(detector->pattern_buffer, 0, sizeof(detector->pattern_buffer));
    detector->pattern_index = 0;
}

void decodeBarcode(BarcodeDetector *detector) {
    // Initialise pattern help variables and strings
    int group_size = 9;
    char ast[] = "tStsTsTst";
    char new_pattern_buffer[3];
    char display_char[1];
    int new_pattern_length = 0;
    char patterns[26][10] = {
        "TststStsT", "tsTstStsT", "TsTstStst", "tstsTStsT", "TstsTStst", "tsTsTStst", "tststSTsT", "TststSTst", "tsTstSTst", "tstsTSTst", "TstststST", "tsTststST", "TsTststSt", "tstsTstST", "TstsTstSt", "tsTsTstSt", "tststsTST", "TststsTSt", "tsTstsTSt", "tstsTsTSt", "TStststsT", "tSTststsT", "TSTststst", "tStsTstsT", "TStsTstst", "tSTsTstst"
    };

    // Increment and ignore space between characters
    for (int i = 0; i < strlen(detector->pattern_buffer); i += group_size + 1) {

        // Copy current set of 9 characters from pattern_buffer to current_group
        char current_group[10];
        strncpy(current_group, detector->pattern_buffer + i, group_size);
        current_group[group_size] = '\0'; 

        // Check for asterisk
        if(strcmp(current_group, ast) == 0) {
            new_pattern_buffer[new_pattern_length] = '*';
            new_pattern_length++;
        }
        else {
            // Check for which letter is found, from A to Z
            for (char letter = 'A'; letter <= 'Z'; ++letter) {
                if (strcmp(current_group, patterns[letter - 'A']) == 0) {
                    new_pattern_buffer[new_pattern_length] = letter;
                    new_pattern_length++;
                    break;
                }
            }
        }

        memset(current_group, 0, sizeof(current_group));
    }

    // Check if barcode matches code 39 patterns
    if (new_pattern_buffer[0] == '*' && new_pattern_buffer[2] == '*') {

        // Check if there is a pattern
        if (new_pattern_length>0) {

            // Display single character output
            strncpy(display_char, new_pattern_buffer + 1, 1);
            display_char[1] = '\0';
            printf("Barcode detected and decoded into: %s\n", display_char);
        }
    } else {
        printf("Misshapen barcode");
    }

    // Reset buffers
    memset(new_pattern_buffer, 0, sizeof(new_pattern_buffer));
    memset(display_char, 0, sizeof(display_char));

    
    
}

// End low barcode sequence
void finishLowBarcode(BarcodeDetector *detector) {

    // If end sequence detected, end scan
    if (detector->low_duration > 10 && detector->pattern_index > 4) {
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

    // Increment and reset durations
    detector->high_duration++; 
    detector->low_duration = 0; 

}

// End high barcode sequence
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

    // Increment and reset durations
    detector->low_duration++; 
    detector->high_duration = 0; 
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
    // Check for started barcode and if there is a line
    if (detector->barcode_started == 0 && sensor_data>=detector->line_threshold) {
        startBarcode(detector);
    }
    // Check for lines
    else if (sensor_data>=detector->line_threshold) {

        // If there was a space prior to this state
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

    // Check if data is below the line threshold 
    if(sensor_data < detector->line_threshold) {

        // Check if barcode has started scanning
        if (detector->barcode_started == 1) {

            // Check if there had been a line prior to this state
            if (detector->high_duration > 0) {
                finishHighBarcode(detector);
            }
            else {

                // End the barcode if sequence detected
                if (detector->low_duration > 10) {
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





