add_executable(irsensor
    irsensor.c
)

pico_enable_stdio_usb(irsensor 1)
pico_enable_stdio_uart(irsensor 0)

target_link_libraries(irsensor 
                pico_stdlib
                hardware_gpio
                hardware_adc)

# Enable USB mass storage support for easy programming (Booting with Pico USB mode)
pico_add_extra_outputs(irsensor)

# enable usb output, enable uart output
pico_enable_stdio_usb(irsensor 1)