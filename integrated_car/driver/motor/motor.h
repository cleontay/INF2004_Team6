/**
 * @brief Callback function for the interrupt request (IRQ).
 *
 * This function is called when an interrupt event occurs on a GPIO pin.
 *
 * @param gpio The GPIO pin number that triggered the interrupt.
 * @param events The events that triggered the interrupt.
 */
void irq_callback(uint gpio, uint32_t events);

/**
 * @brief Move the wheel in a specified direction.
 *
 * This function initializes the GPIO pin for the wheel, sets its direction,
 * and turns it on.
 *
 * @param pin The GPIO pin number for the wheel.
 * @param rotaryDirection The direction of rotation for the wheel (true for clockwise, false for counterclockwise).
 */
void moveWheel(uint pin, bool rotaryDirection){
    gpio_init(pin);
    gpio_set_dir(pin, rotaryDirection);
    gpio_put(pin, 1);
}

/**
 * @brief Reset the wheel to its initial state.
 *
 * This function turns off the PWM signals for all wheels, disables the interrupt
 * requests for the left and right IR pins, and sets the interrupt callback function.
 */
void resetWheel(){
    gpio_put(LEFT_WHEEL_PWM_1, 0);
    gpio_put(LEFT_WHEEL_PWM_2, 0);
    gpio_put(RIGHT_WHEEL_PWM_1, 0);
    gpio_put(RIGHT_WHEEL_PWM_2, 0);
    gpio_set_irq_enabled_with_callback(LEFT_IR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &irq_callback);
    gpio_set_irq_enabled_with_callback(RIGHT_IR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &irq_callback);
}

/**
 * @brief Switch the interrupt request (IRQ) for the wheel.
 *
 * This function enables or disables the interrupt requests for the left and right
 * IR pins based on the specified wheel.
 *
 * @param wheel_irq The wheel to switch the IRQ for (LEFT or RIGHT).
 */
void wheel_irq_switch(int wheel_irq){
    if(wheel_irq == LEFT){
        gpio_set_irq_enabled_with_callback(LEFT_IR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &irq_callback);
        gpio_set_irq_enabled_with_callback(RIGHT_IR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &irq_callback);
    }
    else if (wheel_irq == RIGHT){
        gpio_set_irq_enabled_with_callback(LEFT_IR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &irq_callback);
        gpio_set_irq_enabled_with_callback(RIGHT_IR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &irq_callback);
    }
}

/**
 * @brief Set the speed of the wheel.
 *
 * This function configures the GPIO pin for PWM, sets the clock divider and wrap
 * values for the PWM signal, and sets the channel level based on the specified speed.
 *
 * @param pin The GPIO pin number for the wheel.
 * @param channel The PWM channel for the wheel.
 * @param speed The speed of the wheel (higher values for slower speed).
 */
void wheelSpeed(uint pin, uint channel, uint speed){
    gpio_set_function(pin, GPIO_FUNC_PWM);
    
    uint slice_num = pwm_gpio_to_slice_num(pin);
    
    // debouncing straight
    pwm_set_clkdiv(slice_num, 100);//125Mhz / 100 = 1.25Mhz = 1250000hz
    pwm_set_wrap(slice_num, 31250);//1250000hz / 6250 = 25hz
    pwm_set_chan_level(slice_num, channel, 31250/speed);
    pwm_set_enabled(slice_num, true);
}