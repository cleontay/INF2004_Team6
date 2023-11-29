// barcode2.h

#ifndef BARCODE2_H
#define BARCODE2_H

#include <stdio.h>
#include <stdbool.h>

// Declaring barcode variables
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

#endif // BARCODE2_H