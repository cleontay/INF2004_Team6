/**
 * @file ssi.h
 * @brief Server Side Includes (SSI) functionality for the integrated car driver WiFi module.
 *
 * This file contains the implementation of the SSI handler and initialization function for the integrated car driver WiFi module.
 * SSI allows dynamic content generation in HTML pages served by the HTTP server.
 * The SSI handler retrieves values for specific tags and inserts them into the HTML response.
 * The tags supported in this implementation are "volt", "temp", and "led".
 *
 * @see httpd.h
 * @see cyw43_arch.h
 * @see adc.h
 */

#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"

// SSI tags - tag length limited to 8 bytes by default
const char * ssi_tags[] = {"volt","temp","led"};

/**
 * @brief SSI handler function.
 *
 * This function is responsible for handling the SSI requests and inserting the corresponding values into the HTML response.
 *
 * @param iIndex The index of the SSI tag.
 * @param pcInsert Pointer to the buffer where the SSI value should be inserted.
 * @param iInsertLen The length of the buffer.
 * @return The length of the inserted value.
 */
u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
    size_t printed;

    switch (iIndex) {
        case 0: // voltage
        {
            const float voltage = adc_read() * 3.3f / (1 << 12);
            printed = snprintf(pcInsert, iInsertLen, "%f", voltage);
        }
        break;
        case 1: // temp
        {
            const float voltage = adc_read() * 3.3f / (1 << 12);
            const float tempC = 27.0f - (voltage - 0.706f) / 0.001721f;
            printed = snprintf(pcInsert, iInsertLen, "%f", tempC);
        }
        break;
        case 2: // led
        {
            bool led_status = cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN);
            if(led_status == true){
                printed = snprintf(pcInsert, iInsertLen, "ON");
            }
            else{
                printed = snprintf(pcInsert, iInsertLen, "OFF");
            }
        }
        break;
        default:
            printed = 0;
            break;
    }

  return (u16_t)printed;
}

/**
 * @brief Initialize the SSI handler.
 *
 * This function initializes the SSI handler by setting up the ADC and registering the SSI handler function with the HTTP server.
 * It also sets up the SSI tags that can be used in the HTML pages.
 */
void ssi_init() {
  // Initialise ADC (internal pin)
  adc_init();
  adc_set_temp_sensor_enabled(true);
  adc_select_input(4);

  http_set_ssi_handler(ssi_handler, ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
}