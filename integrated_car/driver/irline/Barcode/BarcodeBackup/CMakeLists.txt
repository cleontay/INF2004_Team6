add_executable(BarcodeBackup main.c barcode2.c)

# pull in common dependencies
target_link_libraries(BarcodeBackup pico_stdlib)
target_link_libraries(BarcodeBackup pico_stdlib hardware_pwm)
target_link_libraries(BarcodeBackup pico_stdlib hardware_adc)

# enable usb output, enable uart output
pico_enable_stdio_usb(BarcodeBackup 1)
pico_enable_stdio_uart(BarcodeBackup 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(BarcodeBackup)

# add url via pico_set_program_url
example_auto_set_url(BarcodeBackup)