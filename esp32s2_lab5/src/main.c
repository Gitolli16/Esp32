#include <stdio.h>
#include "driver/ledc.h"
#include <unistd.h>
#include <rom/gpio.h>
#include <driver/gpio.h>
#include "esp_log.h"
#include <stdlib.h>

#define LED11       11 //g


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