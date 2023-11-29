
/**
 * @brief Callback function for the echo pin interrupt.
 * 
 * This function is called by the echo_interrupt_handler function.
 * It is registered as the callback function for the echo pin interrupt.
 * 
 * @param gpio The GPIO pin number that triggered the interrupt
 * @param events The interrupt events that occurred
 */
void irq_callback(uint gpio, uint32_t events) {
    // Put the GPIO event(s) that just happened into event_str
    // so we can print it
    if(gpio == ECHO_PIN){
        echo_interrupt_handler(gpio, events);
    }
    else if (gpio == IR_SENSOR_PIN){
        IR_sensor_handler(gpio, events);
    }
    else{
        wheel_distance_calculator(gpio, events);
    }
}