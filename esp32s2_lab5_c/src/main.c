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

int state = 0;
QueueHandle_t interputQueue;

static void IRAM_ATTR gpio_interrupt_handler(void *args)
{  
    int pinNumber = (int)args;
    xQueueSendFromISR(interputQueue, &pinNumber, NULL);
}

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

    int pinNumber2, count, state = 0;

    while (true)
    {   
        for (int j = 0; j < 10; j++) {
            for (int k = 0; k < 7; k++)
            {
                gpio_set_level(valot[j][k], 0);

            }
            if (xQueueReceive(interputQueue, &pinNumber2, 0)) { 
                printf("GPIO %d was pressed %d times. The state is %d\n", pinNumber2, count++, gpio_get_level(INPUT_PIN));
                state++;
                while (!gpio_get_level(pinNumber2))
                {
                    for (int l = 0; l < 7; l++)
                    {
                        gpio_set_level(valot[9][l], 0);
                    }
                }
            }
            usleep(1000000);
            for (int k = 0; k < 7; k++)
            {
                gpio_set_level(valot[j][k], 1);
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
    gpio_pulldown_en(INPUT_PIN);
    gpio_pullup_dis(INPUT_PIN);
    gpio_set_intr_type(INPUT_PIN, GPIO_INTR_POSEDGE);

    interputQueue = xQueueCreate(1, sizeof(int));
    xTaskCreate(LED_Control_Task, "LED_Control_Task", 2048, NULL, 1, NULL);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(INPUT_PIN, gpio_interrupt_handler, (void *)INPUT_PIN);
}