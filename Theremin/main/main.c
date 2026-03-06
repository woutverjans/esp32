#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "driver/ledc.h"

//Setup voor de sensor op GPIO36
#define CHANNEL_1 ADC1_CHANNEL_0
#define RESOLUTIE ADC_WIDTH_BIT_12
#define VERZWAKKING ADC_ATTEN_DB_11
#define ADC_UNIT ADC_UNIT_1

//Setup voor de pwm uitgang
ledc_timer_config_t ledc_timer = 
    { 
        .duty_resolution = LEDC_TIMER_6_BIT,
        .freq_hz = 1000, 
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .clk_cfg = LEDC_AUTO_CLK,
    };
ledc_channel_config_t ledc_channel = {
    .gpio_num = 13,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .intr_type = LEDC_INTR_DISABLE,
    .timer_sel = LEDC_TIMER_0,
    .duty = 32,
    .hpoint = 0
};

void app_main(void){
    adc1_config_width(RESOLUTIE);
    adc1_config_channel_atten(CHANNEL_1, VERZWAKKING);

    ledc_timer_config(&ledc_timer);
    ledc_channel_config(&ledc_channel);

    while(1){
        vTaskDelay(pdMS_TO_TICKS(100));
        int sensor = adc1_get_raw(CHANNEL_1);
        int freq = 200 + (sensor * 1800 / 4095);//Mapping
        ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, freq);
    }
}
