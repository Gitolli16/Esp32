#include <stdio.h>
#include "driver/ledc.h"
#include <unistd.h>
#include <rom/gpio.h>
#include <driver/gpio.h>
#include "esp_log.h"
#include <stdlib.h>



int arr_length(int arr[])
{
    int i;
    int count = 0;
    for(i=0; arr[i]!='\0'; i++)
    {
        count++;
    }
    return count;
}


void app_main() 
{
    int ledList[] = {1, 2, 7, 8, 10, 11, 13};

    int valot[][7] = {
        {10, 13},
        {1, 7, 8, 13, 11},
        {1, 10, 11, 13, 8},
        {2, 11, 13, 10},
        {1, 2, 8, 10, 11},
        {1, 2, 7, 8, 10, 11},
        {1, 10, 13},
        {1, 2, 7, 8, 10, 11, 13},
        {1, 2, 8, 10, 11, 13},
        {1, 2, 7, 8, 10, 13}
    };

    for (int i = 0; i <= 7; i++) {
        gpio_pad_select_gpio(ledList[i]);
        gpio_set_direction(ledList[i], GPIO_MODE_OUTPUT);
        gpio_set_level(ledList[i], 1);
    }

    while(1) {
        for (int j = 0; j < 10; j++) {
            for (int k = 0; k < 7; k++)
            {
                gpio_set_level(valot[j][k], 0);
            }
            usleep(1000000);
            for (int k = 0; k < 7; k++)
            {
                gpio_set_level(valot[j][k], 1);
            }
        }
    }
}

