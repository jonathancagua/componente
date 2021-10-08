/**
 * @file bme280.h
 * @author jonathan.cagua@gmail.com
 * @brief contiene las funciones principales a usar por bme_core
 * @version 0.1
 * @date 2021-10-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef BME280_INC_BME28_H_
#define BME280_INC_BME28_DEF_H_

#include "bme280_def.h"
int8_t bme280_init(struct bme280_dev *dev);
int8_t bme280_config_set(struct bme280_dev *dev, struct bme280_enable *config);
int8_t bme280_mode_set(struct bme280_dev *dev, uint8_t sensor_mode);
int8_t bme280_data_get(struct bme280_dev *dev, struct bme280_data *dev_data);
#endif /* BME280_INC_BME28_DEF_H_ */
