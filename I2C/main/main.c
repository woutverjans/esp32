/*
Gebruikte I2C sensor: HS4013
https://be.farnell.com/renesas/hs4013/humidity-temp-sensor-40-to-125deg/dp/3972866
*/

#include <stdio.h>
#include "my_I2C.h"

void app_main(void){
    //structs maken voor de sensorgegevens en meetdata
    hs4013_handle_t sensor;
    hs4013_data_t meting;

    hs4013_init(&sensor);

    while (1) { //meetlus, ongeveer elke 2 seconden
        hs4013_read(&sensor, &meting);

        printf("T=%.2f C | RH=%.2f %%\n",
               meting.temperature_c,
               meting.humidity_pct);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}