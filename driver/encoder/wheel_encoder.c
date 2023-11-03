
void gpio_event_string(char *buf, uint32_t events);

void gpio_callback(uint gpio, uint32_t events) {
    // Put the GPIO event(s) that just happened into event_str
    // so we can print it
    gpio_event_string(event_str, events);
    if(strcmp(event_str, "EDGE_RISE") == 0 && !riseFlag){
        rotaryDetectionCount++;
        riseFlag = true;
    }
    else if(strcmp(event_str, "EDGE_FALL") == 0  && riseFlag){
        riseFlag = false;
    }

    if(rotaryDetectionCount == SINGLELOOP){
        //1 round completed
        rotaryDetectionCount = 0;
        totalDistanceTravaled += LOOPCIRCUMFERENCEINCM;
    }
}

static const char *gpio_irq_str[] = {
        "LEVEL_LOW",  // 0x1
        "LEVEL_HIGH", // 0x2
        "EDGE_FALL",  // 0x4
        "EDGE_RISE"   // 0x8
};
