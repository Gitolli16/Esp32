#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <rom/gpio.h>
#include <stdlib.h>
#include <unistd.h>

#define INPUT_PIN 15
#define LED_PIN 3


void LED_Control_Task(void *params)
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

    int state = 0;
    int flag = 0;

    while (1) {
        if (state % 2 == 0) {
            for (int k = 0; k < 7; k++)
            {
                gpio_set_level(valot[9][k], 0);
                printf("state 0\n");
                while (gpio_get_level(INPUT_PIN) == 0)
                {
                    printf("state 0 while\n");
                    if (gpio_get_level(INPUT_PIN) == 1)
                    {
                        state++;
                        flag++;
                        printf("state 0 while-if %d\n", state);
                        break;

                    }
                }
                if (flag > 0) {
                    for (int k = 0; k < 7; k++)
                    {
                        gpio_set_level(valot[9][k], 0);
                    }
                    flag = 0;
                    break;
                }
            }
        }
        if (state % 2 == 1) {
            for (int j = 0; j < 10; j++) {
                for (int k = 0; k < 7; k++) {
                    gpio_set_level(valot[j][k], 0);
                    printf("state 1\n");
                }
                while (gpio_get_level(INPUT_PIN) == 0)
                {
                    printf("state 1 while\n");
                    if (gpio_get_level(INPUT_PIN) == 1)
                    {
                        state++;
                        printf("state 1 while-if %d\n", state);
                        flag++;
                        break;
                    }
                }
                usleep(500000);
                for (int k = 0; k < 7; k++)
                {
                    gpio_set_level(valot[j][k], 1);
                }
                if (flag > 0) {
                    flag = 0;
                    break;
                }
       
            }
            }
        }
}

void app_main()
{
    gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(INPUT_PIN);
    gpio_set_direction(INPUT_PIN, GPIO_MODE_INPUT);

    
    xTaskCreate(LED_Control_Task, "LED_Control_Task", 2048, NULL, 1, NULL);

}




    