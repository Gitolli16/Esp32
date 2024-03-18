#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include <unistd.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <rom/gpio.h>
#include <driver/gpio.h>
#include "esp_log.h"
#include "esp_adc_cal.h"
#include <stdlib.h>
#include "driver/adc.h"



#define LED1        1  //a
#define LED2        2  //f
#define LED7        7  //e
#define LED8        8  //d
#define LED10       10 //c
#define LED11       11 //g
#define LED13       13 //b




void app_main() {

        gpio_pad_select_gpio(LED11);
        gpio_set_direction(LED11, GPIO_MODE_OUTPUT);
        gpio_set_level(LED11, 1);

    int ledList[] = {1, 2, 7, 8, 10, 13};

    for (int j = 0; j <= 5; j++) {
        gpio_pad_select_gpio(ledList[j]);
        gpio_set_direction(ledList[j], GPIO_MODE_OUTPUT);
        gpio_set_level(ledList[j], 1);
    }

    while(1) {
        for (int i = 0; i <= 5; i++) {
            
            gpio_set_level(ledList[i], 0);
            gpio_set_level(LED11, 0);
            usleep(1000000);
            gpio_set_level(ledList[i], 1);
            gpio_set_level(LED11, 1);
            usleep(1000000);
            
        }
    }
}