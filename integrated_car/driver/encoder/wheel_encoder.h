/**
 * @brief Calculates the distance traveled by the wheel based on rotary encoder events.
 *
 * This function is called whenever a rising or falling edge is detected on the specified GPIO pin.
 * It updates the `rotaryDetectionCount` and `riseFlag` variables accordingly, and calculates the
 * total distance traveled by the wheel when a full rotation is detected.
 *
 * @param gpio The GPIO pin number.
 * @param events The events bitmask indicating the type of edge detected.
 */
void wheel_distance_calculator(uint gpio, uint32_t events) {
    if((events & GPIO_IRQ_EDGE_RISE)){
        rotaryDetectionCount++;
        riseFlag = true;
    }
    else if((events & GPIO_IRQ_EDGE_FALL)){
        riseFlag = false;
        notch_count += 1;
    }

    if(rotaryDetectionCount == SINGLELOOP){
        //1 round completed
        rotaryDetectionCount = 0;
        totalDistanceTravaled += LOOPCIRCUMFERENCEINCM;
    }
}