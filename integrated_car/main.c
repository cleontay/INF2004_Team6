/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 * @file main.c
 * @brief Integrated Car Main Code
 *
 * This file contains the main code for an integrated car project. It includes dependencies for various modules such as WiFi, motor control, IR sensors, ultrasonic sensors, barcode detection, and magnometer. The code initializes these modules and defines tasks for each module's functionality.
 *
 * The main tasks include:
 * - Web server task: Handles the WiFi connection and initializes the web server for controlling the car.
 * - Magnometer task: Reads data from the magnometer sensor periodically.
 * - Motor task: Controls the motor movement based on input from IR sensors and ultrasonic sensors.
 * - Ultrasonic task: Triggers ultrasonic pulses and measures the distance of objects.
 * - Barcode task: Detects barcodes using an IR sensor.
 * - IR line task: Determines the motor state based on input from IR sensors.
 * 
 */

// Main Dependancies
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include <stdbool.h>
#include <stdint.h>

// Wifi  Dependancies
#include "lwip/apps/httpd.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"

// FreeRTOS Dependancies
#include "FreeRTOS.h"
#include "task.h"
#include "message_buffer.h"
#include "queue.h"

// Motor Dependancies
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include <math.h>

// IR Dependancies
#include "hardware/adc.h"

#include "variable.h"

#define MAX_DISTANCE_CM 400
#define TIMEOUT_US 50000
#define SPEED_OF_SOUND 34300

bool object_detected = false;

// External Drivers 
#include "driver/encoder/wheel_encoder.h"
#include "driver/irline/Barcode/barcode.h"
#include "driver/irline/Line/irsensor.h"
#include "driver/magnometer/magnometer.h"
#include "driver/motor/motor.h"
#include "driver/ultrasonic/ultrasonic.h"

#include "interrupt.h"

mag_t mag;
int motor_mode = 0;
//for straight line debounce
int current_speed = 2;

//for echo
// int current_speed = 1;

// for turn
// int current_speed = 1;

bool mapping_flag = false;
int notch = 0;
// int wheel_speed = 1;

void web_task(__unused void *params)
{
    // Initialise ip variables
    ip_addr_t ip;
    ip_addr_t netmask;
    ip_addr_t gateway;

    ip4addr_aton(IP_ADDRESS, &ip);
    ip4addr_aton(NETMASK, &netmask);
    cyw43_arch_init();

    // Set hostname and network
    cyw43_arch_enable_sta_mode();
    cyw43_arch_lwip_begin();
    struct netif *n = &cyw43_state.netif[CYW43_ITF_STA];
    netif_set_hostname(n, "Group6Car");
    dhcp_stop(n);
    netif_set_addr(n, &ip, &netmask, &gateway);
    netif_set_up(n);
    cyw43_arch_lwip_end();

    // Connect to the WiFI network - loop until connected
    while (cyw43_arch_wifi_connect_timeout_ms(
               WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0)
    {
        printf("Attempting to connect...\n");
    }
    // Print a success message once connected
    printf("Connected! \n");

    // Initialise web server
    httpd_init();
    printf("Http server initialised\n");
    printf("Starting server at %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));

    // Configure SSI and CGI handler
    ssi_init();
    printf("SSI Handler initialised\n");
    cgi_init();
    printf("CGI Handler initialised\n");

    // Infinite loop
    while (true)
    {
        vTaskDelay(100);
    }

    cyw43_arch_deinit();
}

void mag_task(__unused void *params){
    printf("Magnometer Task Starting\n");

    // Keep reading magnometer
    while (true){        
        lsm303dlh_read_mag(&mag);
        // printf("Direction: %f\n", get_angle(mag.x, mag.y));
        vTaskDelay(REFRESH_PERIOD);
    }
}

void motor_task(__unused void *params){
    int wheel_irq = LEFT;
    // bool done = false;

    // if(mapping_flag){
    // }
    // else{

        wheelSpeed(RIGHT_WHEEL_PWM_PIN, PWM_CHAN_B, current_speed);
        // for straught debounce
        wheelSpeed(LEFT_WHEEL_PWM_PIN, PWM_CHAN_A, current_speed -1);
        // wheelSpeed(LEFT_WHEEL_PWM_PIN, PWM_CHAN_A, current_speed);

        gpio_set_irq_enabled_with_callback(LEFT_IR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &irq_callback);

        while(true){
            vTaskDelay(100);
            // Reverse if object
            if(object_detected){
                motor_mode = REVERSE;
                resetWheel();
            }
            else{
                motor_mode = FORWARD;
                resetWheel();
            }
            // Check notch if more than count or 0
            if(notch > notch_count || notch == 0){
                // printf("Notch: %i\n", notch_count);
                //original
                switch(motor_mode){
                    case STOP:
                        //STOP
                        resetWheel();
                        break;
                    case FORWARD:
                        //FORWARD
                        moveWheel(LEFT_WHEEL_PWM_2, GPIO_OUT);
                        moveWheel(RIGHT_WHEEL_PWM_2, GPIO_OUT);
                        if(wheel_irq != LEFT){
                            wheel_irq_switch(LEFT);
                        }
                        break;
                    case LEFT:
                        //LEFT
                        moveWheel(RIGHT_WHEEL_PWM_2, GPIO_OUT);
                        moveWheel(LEFT_WHEEL_PWM_2, GPIO_IN);
                        notch =1;
                        if(wheel_irq != RIGHT){
                            wheel_irq_switch(RIGHT);
                        }
                        break;
                    case RIGHT:
                        //RIGHT
                        moveWheel(LEFT_WHEEL_PWM_2, GPIO_OUT);
                        moveWheel(RIGHT_WHEEL_PWM_2, GPIO_IN);
                        notch = 1;
                        if(wheel_irq != LEFT){
                            wheel_irq_switch(LEFT);
                        }
                        break;
                    case REVERSE:
                        //REVERSE
                        moveWheel(LEFT_WHEEL_PWM_1, GPIO_OUT);
                        moveWheel(RIGHT_WHEEL_PWM_1, GPIO_OUT);
                        motor_mode = STOP;
                        if(wheel_irq != LEFT){
                            wheel_irq_switch(LEFT);
                        }
                        break;
                    default:
                        break;
                }
            }
            // Reset notch
            else{
                notch = 0;
                notch_count = 0;
                motor_mode = FORWARD;
            }
        }
    // }
}

void ultrasonic_task(__unused void *params){

    ultrasonic_init();
    // Keep checking ultrasonic
    while (1) {
        vTaskDelay(100);
        trigger_pulse();
    }
}

void barcode_task(__unused void *params){
    adc_set_temp_sensor_enabled(true);
    BarcodeDetector barcodeDetector;
    initBarcodeDetector(&barcodeDetector);

    // Initialize the GPIO pin for the IR sensor as an input
    gpio_init(IR_SENSOR_PIN);
    gpio_set_dir(IR_SENSOR_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(IR_SENSOR_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &irq_callback);

    while(1) {
        // Manage pin states and consistently update barcode scanner
        if (isHigh == true) {
            isHighState(&barcodeDetector);
        }
        else {
            isLowState(&barcodeDetector);
        }
        
        sleep_ms(200);
    }
}

void irline_task(__unused void *params){
    init_sensors();
    // bool noread = false;
    enum MotorState irMotorState;

    while (1) {
        if(notch_count == 0){

            vTaskDelay(100);

            irMotorState = determine_motor_state();

            // for all motor states. Modify when lines found.
            switch (irMotorState) {
                case NO_LINE:
                    motor_mode = FORWARD;
                    printf("FORWARD\n");
                    break;
                case LEFT_LINE:
                    motor_mode = RIGHT;
                    printf("RIGHT\n");
                    break;
                case RIGHT_LINE:
                    motor_mode = LEFT;
                    printf("LEFT\n");
                    break;
                case BOTH_LINES:
                    motor_mode = STOP;
                    printf("STOP\n");
                    break;
                default:
                    break;
            }

        }
    }
}


void vLaunch(void)
{
    // Initialise and create tasks for each driver for FreeRTOS

    // Web server task init
    TaskHandle_t web_task_handler;
    xTaskCreate(web_task,
                "webserverThread",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 1UL,
                &web_task_handler);

    // Magnometer task init
    TaskHandle_t mag_task_handler;
    xTaskCreate(mag_task,
            "MagThread",
            configMINIMAL_STACK_SIZE,
            NULL,
            tskIDLE_PRIORITY,
            &mag_task_handler);

    // Motor task init
    TaskHandle_t motor_task_handler;
    xTaskCreate(motor_task,
            "MotorThread",
            configMINIMAL_STACK_SIZE,
            NULL,
            tskIDLE_PRIORITY,
            &motor_task_handler);

    // Ultrasonic task init
    TaskHandle_t ultrasonic_task_handler;
    xTaskCreate(ultrasonic_task,
            "UltraThread",
            configMINIMAL_STACK_SIZE,
            NULL,
            tskIDLE_PRIORITY,
            &ultrasonic_task_handler);

    // Barcode task init
    TaskHandle_t barcode_task_handler;
    xTaskCreate(barcode_task,
            "barcodeThread",
            configMINIMAL_STACK_SIZE,
            NULL,
            tskIDLE_PRIORITY,
            &barcode_task_handler);

    // IR line sensor task init
    TaskHandle_t irline_task_handler;
    xTaskCreate(irline_task,
            "irlineThread",
            configMINIMAL_STACK_SIZE,
            NULL,
            tskIDLE_PRIORITY,
            &irline_task_handler);

    vTaskStartScheduler();
}

int main(void)
{
    // Initialise pins and communications
    init_i2c_default();
    stdio_init_all();

    sleep_ms(5000);

    printf("Program Starting\n");

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }

    for (int i = 10; i <= 13; i++){
        gpio_init(i);
        gpio_set_dir(i, GPIO_OUT);
        gpio_put(i, 1);
    }

    for (int i = 20; i <= 22; i++){
        gpio_init(i);
        gpio_set_dir(i, GPIO_OUT);
        gpio_put(i, 1);
    }

    lsm303dlh_mag_setup();

    adc_init();

    vLaunch();
    /* Configure the hardware ready to run the demo. */
    const char *rtos_name;
    rtos_name = "FreeRTOS";

    printf("Starting %s on core 0:\n", rtos_name);
    return 0;
}
