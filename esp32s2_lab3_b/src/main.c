
#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include <unistd.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <rom/gpio.h>
#include <driver/gpio.h>

#define LED1                    1
#define LED2                    2
#define LED3                    3
#define BUTTON                  19
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (18) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_FREQUENCY          (4000) // Frequency in Hertz. Set frequency at 4 kHz

int ledc_duty = 1;
int button_state = 1;

static void example_ledc_init(void)
{
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_DUTY_RES,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 4 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void fade(void *pvParams)
{
    example_ledc_init();
    gpio_pad_select_gpio(LED1);
    gpio_set_direction(LED1, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(LED2);
    gpio_set_direction(LED2, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(LED3);
    gpio_set_direction(LED3, GPIO_MODE_OUTPUT);    
    int duty_add = 200;
    while (1)
    {
        gpio_set_level(LED1, 0);
        gpio_set_level(LED2, 0);
        gpio_set_level(LED3, 0);
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, ledc_duty));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
        if (ledc_duty >= 7800) {
            duty_add = -duty_add;
        }
        else if (ledc_duty <= 0) {
            duty_add = -duty_add;
        }
        switch(button_state % 3) {
            case 0:
                gpio_set_level(LED1, 1);
                ledc_duty = ledc_duty + duty_add;
                usleep(100000);
                break;
            case 1:
                gpio_set_level(LED2, 1);
                ledc_duty = ledc_duty + (duty_add*2);
                usleep(100000);
                break;
            case 2:
                gpio_set_level(LED3, 1);
                ledc_duty = ledc_duty + (duty_add*3);
                usleep(100000);
                break;
            }
    }
}

void button(void *pvParams) {
    while (1) {
        gpio_pad_select_gpio(BUTTON);
        gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
        gpio_pad_select_gpio(LED1);
        gpio_set_direction(LED1, GPIO_MODE_OUTPUT);
        if (gpio_get_level(BUTTON)) {
            button_state++;
            usleep(1000000);
        }
    }
}


void app_main() {

    xTaskCreatePinnedToCore(&fade, "STATE_BUTTON", 512, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(&button, "STATE_BUTTON", 512, NULL, 5, NULL, 1);

}