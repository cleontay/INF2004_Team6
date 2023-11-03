add_executable(Barcode main.c barcode.c)

# pull in common dependencies
target_link_libraries(Barcode pico_stdlib)
target_link_libraries(Barcode pico_stdlib hardware_pwm)
target_link_libraries(Barcode pico_stdlib hardware_adc)

# enable usb output, enable uart output
pico_enable_stdio_usb(Barcode 1)
pico_enable_stdio_uart(Barcode 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(Barcode)

# add url via pico_set_program_url
example_auto_set_url(Barcode)