#include <stdio.h>
#include "my_I2C.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c.h"

static const char *TAG = "HS4013";

//sensor initialiseren
esp_err_t hs4013_init(hs4013_handle_t *sensor){
    sensor->i2c_port = I2C_NUM_0;
    sensor->dev_addr = 0x44;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };

    ESP_ERROR_CHECK(i2c_param_config(sensor->i2c_port, &conf));
    return i2c_driver_install(sensor->i2c_port,conf.mode,0, 0, 0);
}

//sensor uitlezen
esp_err_t hs4013_read(hs4013_handle_t *sensor, hs4013_data_t *data){
    uint8_t rx[4];

    //starten leesfase, p18 datasheet (0xA7 commando sturen)
    uint8_t cmd[2] = {0xA7, 0x00};


    esp_err_t ret = i2c_master_write_read_device(
    sensor->i2c_port,
    sensor->dev_addr,
    cmd,
    sizeof(cmd),
    rx,
    sizeof(rx),
    pdMS_TO_TICKS(100)
    );
    /*esp_err_t ret = i2c_master_write(
        sensor->i2c_port,
        cmd,
        sizeof(cmd),
        pdMS_TO_TICKS(100)
    );*/

    if (ret != ESP_OK){
        return ret;
    }

    //lees data (4 bytes)
    i2c_master_write_read_device(
    sensor->i2c_port,
    sensor->dev_addr,
    cmd,
    sizeof(cmd),
    rx,
    sizeof(rx),
    pdMS_TO_TICKS(100)
    );

    /*
    ret = i2c_master_read(
        sensor->i2c_port,
        rx,
        sizeof(rx),
        pdMS_TO_TICKS(100)
    );*/

    if (ret != ESP_OK){
        return ret;
    }

    //uitlezen sensorwaarden
    uint16_t raw_hum =((rx[0] & 0x3F) << 8) | rx[1];
    uint16_t raw_temp =(rx[2] << 6) | (rx[3] >> 2);
    //omzetten naar vochtigheid en temperatuur
    data->humidity_pct =(raw_hum / 16383.0f) * 100.0f;
    data->temperature_c =(raw_temp / 16383.0f) * 165.0f - 40.0f;

    return ESP_OK;
}

//verbinding verbreken (deinitialiseren)
esp_err_t hs4013_deinit(hs4013_handle_t *sensor){
    return i2c_driver_delete(sensor->i2c_port);
}
