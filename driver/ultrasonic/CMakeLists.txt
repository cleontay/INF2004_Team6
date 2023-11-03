add_executable(ultrasonic_sensor
ultrasonic_sensor.c
)

pico_enable_stdio_usb(ultrasonic_sensor 1)
pico_enable_stdio_uart(ultrasonic_sensor 0)

target_link_libraries(ultrasonic_sensor 
                pico_stdlib
                hardware_gpio
                ultrasonic_sensor
                )

# Enable USB mass storage support for easy programming (Booting with Pico USB mode)
pico_add_extra_outputs(ultrasonic_sensor)

# enable usb output, enable uart output
pico_enable_stdio_usb(ultrasonic_sensor 1)