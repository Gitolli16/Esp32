#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <rom/gpio.h>



#define LED_PIN 1
#define BUTTON_PIN 3
#define STATE_PIN 4



volatile int shared = 0;

void buttonPressed() {
    gpio_pad_select_gpio(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    if (gpio_get_level(BUTTON_PIN)) {
        gpio_set_level(LED_PIN, 1);
    } else {
        gpio_set_level(LED_PIN, 0);
    }
}

void beep(int beepTime) {
    gpio_set_level(LED_PIN, 1);
    vTaskDelay(beepTime);
    gpio_set_level(LED_PIN, 0);
    vTaskDelay(beepTime);
}

void ledBlink() {
    gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    beep(20);
    beep(20);
    beep(20);
    beep(50);
    beep(50);
    beep(50);
    beep(20);
    beep(20);
    beep(20);
    vTaskDelay(100);
}

void stateButton(void *pvParams) {
    while(1) {
        gpio_pad_select_gpio(STATE_PIN);
        gpio_set_direction(STATE_PIN, GPIO_MODE_INPUT);
        if (gpio_get_level(STATE_PIN)) {
            shared++;
            vTaskDelay(20);
        }
    }
}

void stateMachine(void *pvParams) {
    while (1)
    {
        switch (shared % 2)
        {
        case 0:
            buttonPressed();
            break;
        case 1:
            ledBlink();
            break;
        }
        
    }
}

void app_main() {

    xTaskCreatePinnedToCore(&stateButton, "STATE_BUTTON", 512, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(&stateMachine, "STATE_MACHINE", 512, NULL, 5, NULL, 1);
}