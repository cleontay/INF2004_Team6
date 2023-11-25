void moveWheel(uint pin, bool rotaryDirection){
    gpio_init(pin);
    gpio_set_dir(pin, rotaryDirection);
    gpio_put(pin, 1);
}

void resetWheel(){
    gpio_put(LEFT_WHEEL_PWM_1, 0);
    gpio_put(LEFT_WHEEL_PWM_2, 0);
    gpio_put(RIGHT_WHEEL_PWM_1, 0);
    gpio_put(RIGHT_WHEEL_PWM_2, 0);
    gpio_set_irq_enabled_with_callback(LEFT_IR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &wheel_encoder_callback);
    gpio_set_irq_enabled_with_callback(RIGHT_IR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &wheel_encoder_callback);
}

void wheel_irq_switch(int wheel_irq){
    if(wheel_irq == LEFT){
        gpio_set_irq_enabled_with_callback(LEFT_IR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &wheel_encoder_callback);
        gpio_set_irq_enabled_with_callback(RIGHT_IR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &wheel_encoder_callback);
    }
    else if (wheel_irq == RIGHT){
        gpio_set_irq_enabled_with_callback(LEFT_IR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &wheel_encoder_callback);
        gpio_set_irq_enabled_with_callback(RIGHT_IR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &wheel_encoder_callback);
    }
}

void wheelSpeed(uint pin, uint channel, uint speed){
    gpio_set_function(pin, GPIO_FUNC_PWM);
    
    uint slice_num = pwm_gpio_to_slice_num(pin);
    
    pwm_set_clkdiv(slice_num, 100);
    pwm_set_wrap(slice_num, 12500);
    pwm_set_chan_level(slice_num, channel, 12500/speed);
    pwm_set_enabled(slice_num, true);
}
