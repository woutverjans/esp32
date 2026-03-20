#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_random.h"
#include "esp_log.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define BOARD_LED 2
#define BTN 12

void app_main(void){
    gpio_set_direction(BOARD_LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(BTN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN, GPIO_PULLUP_ONLY);
    gpio_set_level(BOARD_LED, 0);

    unsigned long current_time = 0;
    unsigned long activation_time = 0;

    int testing = 1;
    printf("starting\n");
    
    while(1){
        vTaskDelay(1); //Voor de watchdog tegen te houden
        current_time = esp_timer_get_time() / 1000;
        int btn_state = gpio_get_level(BTN);
        if(current_time >= activation_time - 10 && current_time <= activation_time + 10){
            gpio_set_level(BOARD_LED, 1);
            testing = 1;
        }
        if(btn_state == 0 && testing == 1){
            testing = 0;
            gpio_set_level(BOARD_LED, 0);

            unsigned long reaction_time = current_time - activation_time;
            printf("reaction time is: %lu\n", reaction_time);

            int delay = rand() % 10 + 1; //random getal tussen 1 en 10 voor te wachten
            activation_time = current_time + delay * 1000;
            printf("please wait for %d seconds!\n", delay);
        }
    }
}
