#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "ultrasonic.h"

int main() {
    stdio_init_all();

    gpio_init(TRIGGER_PIN);
    gpio_set_dir(TRIGGER_PIN, GPIO_OUT);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    gpio_set_irq_enabled_with_callback(ECHO_PIN, GPIO_IRQ_EDGE_RISE, true, echo_interrupt_handler);

    check_and_print_object_detection();

    return 0;
}
