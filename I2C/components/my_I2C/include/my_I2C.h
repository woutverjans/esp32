#ifndef MY_I2C_H
#define MY_I2C_H

#include "driver/i2c.h"
#include "esp_err.h"

//struct voor verbinding maken
typedef struct {
    i2c_port_t i2c_port;
    uint8_t dev_addr;
} hs4013_handle_t;

//struct sensordata
typedef struct {
    float temperature_c;
    float humidity_pct;
} hs4013_data_t;

//functies
esp_err_t hs4013_init(hs4013_handle_t *sensor);
esp_err_t hs4013_read(hs4013_handle_t *sensor, hs4013_data_t *data);
esp_err_t hs4013_deinit(hs4013_handle_t *sensor);

#endif
