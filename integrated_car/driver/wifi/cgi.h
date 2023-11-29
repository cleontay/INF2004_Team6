/**
 * @file cgi.h
 * @brief Header file for CGI handlers used in a WiFi driver application.
 *
 * This file contains the declarations of CGI handlers used in a WiFi driver application.
 * CGI handlers are functions that are executed when a specific CGI request is made.
 * The CGI handlers in this file handle requests related to controlling an LED.
 */
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"

/**
 * @brief CGI handler for LED control.
 *
 * This function is executed when a request for "/led.cgi" is detected.
 * It checks the request parameters and values to determine if the LED should be turned on or off.
 *
 * @param iIndex The index of the CGI request.
 * @param iNumParams The number of parameters in the request.
 * @param pcParam An array of parameter names.
 * @param pcValue An array of parameter values.
 * @return The path of the page to be sent back to the user.
 */
const char * cgi_led_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    // Check if an request for LED has been made (/led.cgi?led=x)
    if (strcmp(pcParam[0] , "led") == 0){
        // Look at the argument to check if LED is to be turned on (x=1) or off (x=0)
        if(strcmp(pcValue[0], "0") == 0)
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        else if(strcmp(pcValue[0], "1") == 0)
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    }
    
    // Send the index page back to the user
    return "/index.shtml";
}

// tCGI Struct
// Fill this with all of the CGI requests and their respective handlers
static const tCGI cgi_handlers[] = {
    {
        // Html request for "/led.cgi" triggers cgi_handler
        "/led.cgi", cgi_led_handler
    },
};

/**
 * @brief Initializes the CGI handlers.
 *
 * This function registers the CGI handlers with the HTTP server.
 */
void cgi_init(void)
{
    http_set_cgi_handlers(cgi_handlers, 1);
}