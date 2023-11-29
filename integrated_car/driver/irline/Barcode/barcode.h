
#ifndef BARCODE_H
#define BARCODE_H

/**
 * @brief Structure to hold the variables and state for barcode detection.
 */
// Initialise variables for barcode detector struct
typedef struct {
    int high_duration;
    int low_duration;
    int barcode_started;
    char pattern_buffer[30];
    int pattern_index;
    int line_threshold;
} BarcodeDetector;

extern bool isHigh;

// Initialise functions
void initBarcodeDetector(BarcodeDetector *detector);
void startBarcode(BarcodeDetector *detector);
void endBarcode(BarcodeDetector *detector);
void finishLowBarcode(BarcodeDetector *detector);
void finishHighBarcode(BarcodeDetector *detector);
void addToHighBarcode(BarcodeDetector *detector);
void addToLowBarcode(BarcodeDetector *detector);
void isHighState(BarcodeDetector *detector);
void isLowState(BarcodeDetector *detector);
void IR_sensor_handler(uint gpio, uint32_t events);

#endif // BARCODE_H

bool isHigh = false;

/**
 * @brief Initializes the barcode detector by setting the initial values of the variables.
 * 
 * @param detector Pointer to the BarcodeDetector structure.
 */void initBarcodeDetector(BarcodeDetector *detector) {
    // Initialize detector settings
    detector->high_duration = 0;
    detector->low_duration = 0;
    detector->barcode_started = 0;
    detector->pattern_index = 0;
    detector->line_threshold = 2000;
}

/**
 * @brief Starts the barcode scanning process by incrementing the high pulse duration and setting the barcode_started flag.
 * 
 * @param detector Pointer to the BarcodeDetector structure.
 */void startBarcode(BarcodeDetector *detector) {
    detector->high_duration++;
    detector->barcode_started = 1;

}

/**
 * Decodes the barcode pattern stored in the given BarcodeDetector object.
 * 
 * @param detector The BarcodeDetector object containing the barcode pattern.
 */
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

/**
 * @brief Finishes the low pulse of the barcode by determining if it is a big space or a small space.
 * 
 * @param detector Pointer to the BarcodeDetector structure.
 */
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
    detector->high_duration++; // Increment the 'high' duration
    detector->low_duration = 0; // Reset the 'low' duration

}

/**
 * @brief Finishes the high pulse of the barcode by determining if it is a big line or a small line.
 * 
 * @param detector Pointer to the BarcodeDetector structure.
 */
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

/**
 * @brief Adds to the high pulse duration of the barcode.
 * 
 * @param detector Pointer to the BarcodeDetector structure.
 */void addToHighBarcode(BarcodeDetector *detector) {
    detector->high_duration++;
}

/**
 * @brief Adds to the low pulse duration of the barcode.
 * 
 * @param detector Pointer to the BarcodeDetector structure.
 */void addToLowBarcode(BarcodeDetector *detector) {
    detector->low_duration++;
    detector->high_duration = 0;
}

/**
 * @brief Handles the high state of the barcode sensor by checking the sensor data and starting or continuing the barcode scanning process.
 * 
 * @param detector Pointer to the BarcodeDetector structure.
 */void isHighState(BarcodeDetector *detector) {
    uint16_t sensor_data = adc_read();
    printf("Barcode ADC HS Value: %d\n", sensor_data);
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

/**
 * @brief Handles the low state of the barcode sensor by checking the sensor data and finishing or continuing the barcode scanning process.
 * 
 * @param detector Pointer to the BarcodeDetector structure.
 */void isLowState(BarcodeDetector *detector) {
    uint16_t sensor_data = adc_read();
    printf("Barcode ADC LS Value: %d\n", sensor_data);
    if(sensor_data < detector->line_threshold) {
        if (detector->barcode_started == 1) {
        if (detector->high_duration > 0) {
            finishHighBarcode(detector);
        }
        else {
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

/**
 * @brief Handler function for the IR sensor events.
 * 
 * @param gpio The GPIO pin number.
 * @param events The events triggered on the GPIO pin.
 */void IR_sensor_handler(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_RISE) {
        isHigh = true;
    }
    else if (events & GPIO_IRQ_EDGE_FALL) {
        isHigh = false;
    }
}

/**
 * @brief Ends the barcode scanning process by checking for the end sequence and resetting the variables.
 * 
 * @param detector Pointer to the BarcodeDetector structure.
 */void endBarcode(BarcodeDetector *detector) {
    detector->pattern_buffer[detector->pattern_index] = '\0';
    detector->barcode_started = 0;
    detector->low_duration = 0;
    if(strlen(detector->pattern_buffer) == 29) {
        decodeBarcode(detector);
    }
    else if(strlen(detector->pattern_buffer)>1) {
        printf("Misshapen barcode: Length\n");
    }
    printf("%s\n", detector->pattern_buffer);
    memset(detector->pattern_buffer, 0, sizeof(detector->pattern_buffer));
    detector->pattern_index = 0;
}


