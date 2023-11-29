
include(pico_sdk_import.cmake)

message("Running makefsdata python script")
execute_process(COMMAND
    py convert_html.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}

)

add_executable(integrated_car
    main.c
    ssi.h
    cgi.h
    variable.h
    interrupt.h
)
    
    target_compile_definitions(integrated_car PRIVATE
        PICO_MAX_SHARED_IRQ_HANDLERS=5
    )
    target_include_directories(integrated_car PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../.. # for our common lwipopts
    
    )
target_link_libraries(integrated_car
    # pico_cyw43_arch_lwip_sys_freertos
    pico_cyw43_arch_lwip_threadsafe_background
    # pico_cyw43_arch_none
    pico_lwip_http
    pico_stdlib
    pico_lwip_iperf
    FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap
    hardware_pwm
    hardware_adc
    hardware_i2c
    hardware_spi
    hardware_gpio
    )
    
pico_enable_stdio_usb(integrated_car 1)
pico_enable_stdio_uart(integrated_car 0)

pico_add_extra_outputs(integrated_car)
# add url via pico_set_program_url
example_auto_set_url(integrated_car)
