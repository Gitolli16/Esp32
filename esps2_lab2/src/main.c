#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <rom/gpio.h>

#define LED_PIN 1

void longBeep() {
    gpio_set_level(LED_PIN,1);
    vTaskDelay(100);
    gpio_set_level(LED_PIN,0);
    vTaskDelay(20);
}

void shortBeep() {
    gpio_set_level(LED_PIN,1);
    vTaskDelay(40);
    gpio_set_level(LED_PIN,0);
    vTaskDelay(20);
}

void led_blink(void *pvParams) {
    gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction (LED_PIN,GPIO_MODE_OUTPUT);
    while (1) {
        longBeep();
        longBeep();
        longBeep();
        shortBeep();
        shortBeep();
        shortBeep();
        longBeep();
        longBeep();
        longBeep();
        vTaskDelay(300);
    }
}

void app_main() {
    xTaskCreate(&led_blink,"LED_BLINK",512,NULL,5,NULL);
}