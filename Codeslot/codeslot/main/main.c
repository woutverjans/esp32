/*
TODO:
- Delen van analoge waarde ipv ranges te gebruiken
- Orderlijkere implementatie (switch case, hele array getallen in ene keer, ...)
*/

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define INPUT ADC1_CHANNEL_0 //GPIO 36
#define RESOLUTIE ADC_WIDTH_BIT_9 //Compimeren voor makkelijk juiste getallen in te geven
#define VERZWAKKING ADC_ATTEN_DB_11
#define ADC_UNIT ADC_UNIT_1

static esp_adc_cal_characteristics_t *adc_chars;
#define ACTUATOR 13
#define BTN 14

void app_main(void){
    adc1_config_width(RESOLUTIE);
    adc1_config_channel_atten(INPUT, VERZWAKKING);

    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));

    esp_adc_cal_characterize(ADC_UNIT, VERZWAKKING, RESOLUTIE, 1100, adc_chars);

    gpio_set_direction(ACTUATOR, GPIO_MODE_OUTPUT);
    gpio_set_direction(BTN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN, GPIO_PULLUP_ONLY);

    int btn_fase = 0; //Gebruiken om bij te houden waar in het programma we zijn (code maken, welk getal en hetzelfde bij code ingeven)
    gpio_set_level(ACTUATOR, 0);
    int digits[4] = {0}; //Array voor de code, initialiseerd op 0000

    int prev_btn = 0;
    int curnt_btn = 0;
    int rising_edge = 0;
    while(1){
        prev_btn = curnt_btn;
        vTaskDelay(pdMS_TO_TICKS(50)); //Tegen debouncen
        curnt_btn = gpio_get_level(BTN);

        if(curnt_btn == 1 && prev_btn == 0){rising_edge = 1;}
        else{rising_edge = 0;}

        int raw_val = adc1_get_raw(INPUT); //waarde van 0 tem 511
        int input_val = raw_val / 60 + 1; //Zet waarde om van 1 tem 9
        printf("Fase = %d en geselcteerd getal: %d\n",btn_fase, input_val);

        if(rising_edge == 1){
            //Begin ingeef fase
            if(btn_fase == 0){
                digits[0] = input_val;
                btn_fase = 1;
                gpio_set_level(ACTUATOR, 0);
            }

            else if(btn_fase == 1){
                digits[1] = input_val;
                btn_fase = 2;
            }

            else if(btn_fase == 2){
                digits[2] = input_val;
                btn_fase = 3;
            }

            else if(btn_fase == 3){
                digits[3] = input_val;
                btn_fase = 4;
            }

            //Begin open fase
            else if(btn_fase == 4){
                if(input_val == digits[0]){
                    btn_fase = 5;
                }
            }
            else if(btn_fase == 5){
                if(input_val == digits[1]){
                    btn_fase = 6;
                }
                else{
                    btn_fase = 4;
                }
            }
            else if(btn_fase == 6){
                if(input_val == digits[2]){
                    btn_fase = 7;
                }
                else{
                    btn_fase = 4;
                }
            }
            else if(btn_fase == 7){
                if(input_val == digits[3]){
                    btn_fase = 0;
                    gpio_set_level(ACTUATOR, 1);
                    printf("SLOT OPENT SLOT OPENT SLOT OPENT SLOT OPENT SLOT OPENT SLOT OPENT SLOT OPENT SLOT OPENT\n");
                }
                else{
                    btn_fase = 4;
                }
            }
        }
    }
}
