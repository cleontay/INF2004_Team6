#define LEFT_WHEEL_PWM_PIN 0
#define RIGHT_WHEEL_PWM_PIN 1
#define LEFT_WHEEL_PWM_1 2
#define LEFT_WHEEL_PWM_2 3
#define RIGHT_WHEEL_PWM_1 4
#define RIGHT_WHEEL_PWM_2 5

#define LEFT_IR_PIN 17
#define RIGHT_IR_PIN 16

#define STOP 0
#define FORWARD 1
#define LEFT 2
#define RIGHT 3
#define REVERSE 4

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
    gpio_set_irq_enabled_with_callback(LEFT_IR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &gpio_callback);
    gpio_set_irq_enabled_with_callback(RIGHT_IR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &gpio_callback);
}

void wheelSpeed(uint pin, uint channel, uint speed){
    gpio_set_function(pin, GPIO_FUNC_PWM);
    
    uint slice_num = pwm_gpio_to_slice_num(pin);
    
    pwm_set_clkdiv(slice_num, 100);
    pwm_set_wrap(slice_num, 12500);
    pwm_set_chan_level(slice_num, channel, 12500/speed);
    pwm_set_enabled(slice_num, true);
}
