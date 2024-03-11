#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <rom/gpio.h>

#define LED_PIN 1
#define BUTTON_PIN 3

void buttonPressed() {
    gpio_pad_select_gpio(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN,GPIO_MODE_INPUT);
    if (gpio_get_level(BUTTON_PIN)) {
        gpio_set_level(LED_PIN,1);
    }
    else {
        gpio_set_level(LED_PIN,0);
    }
}

void led_blink(void *pvParams) {
    gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN,GPIO_MODE_OUTPUT);
    while (1) {
        buttonPressed();
    }
}

void app_main() {
    xTaskCreate(&led_blink,"LED_BLINK",512,NULL,5,NULL);
}