#include <stdio.h>
#include "my_GPIO.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h" 
#include "freertos/task.h"
#define BOARD_LED 2
#define BTN_PIN 4

/*
- Knop lezen
- LED aandrijven
- Flank detectie
- Teller, LED aan na 5 drukken
*/

void func(void){
    gpio_reset_pin(BOARD_LED);
    gpio_set_direction(BOARD_LED, GPIO_MODE_OUTPUT); 
    gpio_reset_pin(BTN_PIN);
    gpio_set_direction(BTN_PIN, GPIO_MODE_INPUT);
    
    gpio_set_level(BOARD_LED, 0);
    int led_state = 0;
    int prev_btn_state = 1; //Initiële button state
    int counter = 0;
    int btn_state = 1;
    while(1){
        prev_btn_state = btn_state;
        btn_state = gpio_get_level(BTN_PIN);
        gpio_set_level(BOARD_LED, led_state);
        if (btn_state == 0 && prev_btn_state == 1){//Falling edge detectie
            counter += 1;
        }
        if (counter >= 5){
            led_state = !led_state;
            counter = 0;
        }
        vTaskDelay(2);
    }
}