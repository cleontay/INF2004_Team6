/**
 * @file variable.h
 * @brief Header file containing various constants and variables used in the integrated car project.
 */

//Wifi Web Server
#define WIFI_SSID "cookiie"
#define WIFI_PASSWORD "rllr4884"
#define IP_ADDRESS "192.168.184.218"
#define NETMASK "255.255.255.0"

//Magnenometer
#define I2C_BAUD 400 
#define REFRESH_PERIOD 500 // ms
#define I2C_PORT_MAGNETOMETER i2c1
#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15

#define CTRL_REG_1 0x20
#define CTRL_REG_4 0x23
#define INTERFACE_A 0x18 // Accelerometr slave address
#define INTERFACE_B 0x1E // Magnetometr slave address
#define MAG_CRA 0x00
#define MAG_CRB 0x01
#define MAG_MR 0x02
#define ACC_REG 0xA8 // data starts on 0x28 - MSb set to 1 (autoincrease on read) -> 0xA8
#define MAG_REG 0x03 // data starts on 0x03 - MSb set to 1 (autoincrease on read) -> 0xA8
#define PI 3.141592653

//Motor Sensors
#define LEFT_WHEEL_PWM_PIN 0
#define RIGHT_WHEEL_PWM_PIN 1
#define LEFT_WHEEL_PWM_1 2
#define LEFT_WHEEL_PWM_2 3
#define RIGHT_WHEEL_PWM_1 4
#define RIGHT_WHEEL_PWM_2 5

#define STOP 0
#define FORWARD 1
#define LEFT 2
#define RIGHT 3
#define REVERSE 4
#define TURNLEFT 5
#define TURNRIGHT 6

#define SPEED_1 1
#define SPEED_2 2
#define SPEED_3 3
#define SPEED_4 4

//Wheel Encoder
#define LEFT_IR_PIN 17
#define RIGHT_IR_PIN 16
#define SINGLELOOP 20
#define LOOPCIRCUMFERENCEINCM 18

int notch_count = 0;
int totalDistanceTravaled = 0;

//FreeRTOS
#ifndef RUN_FREERTOS_ON_CORE
#define RUN_FREERTOS_ON_CORE 0
#endif

#define TEST_TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)
#define mbaTASK_MESSAGE_BUFFER_SIZE (60)

//IR Line Sensors
#define IR_SENSOR1_PIN 26 // assume left
#define IR_SENSOR2_PIN 27 // assume right
#define INTERRUPT_PIN 9
#define SHADOW_THRESHOLD 2000

//Ultrasonic Sensor
#define TRIGGER_PIN 19
#define ECHO_PIN 18
#define SPEED_OF_SOUND_CM_US 0.0343
#define MAX_WAIT_TIME_US 10000
#define OBJECT_DETECTION_THRESHOLD 15.0
#define NUM_AVERAGES 8

//Barcode Sensor
#define IR_SENSOR_PIN 28
