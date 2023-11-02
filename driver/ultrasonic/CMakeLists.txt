add_executable(ultrasonic
    ultrasonic.c
)

pico_enable_stdio_usb(ultrasonic 1)
pico_enable_stdio_uart(ultrasonic 0)

target_link_libraries(ultrasonic 
                pico_stdlib
                hardware_gpio
                )

# Enable USB mass storage support for easy programming (Booting with Pico USB mode)
pico_add_extra_outputs(ultrasonic)

# enable usb output, enable uart output
pico_enable_stdio_usb(ultrasonic 1)