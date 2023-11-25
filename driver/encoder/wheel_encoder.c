static char event_str[128];
bool riseFlag = false;
int rotaryDetectionCount = 0;
int totalDistanceTravaled = 0;

void gpio_event_string(char *buf, uint32_t events);

void wheel_encoder_callback(uint gpio, uint32_t events) {
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

void gpio_event_string(char *buf, uint32_t events) {
    for (uint i = 0; i < 4; i++) {
        uint mask = (1 << i);
        if (events & mask) {
            // Copy this event string into the user string
            const char *event_str = gpio_irq_str[i];
            while (*event_str != '\0') {
                *buf++ = *event_str++;
            }
            events &= ~mask;

            // If more events add ", "
            if (events) {
                *buf++ = ',';
                *buf++ = ' ';
            }
        }
    }
    *buf++ = '\0';
}
