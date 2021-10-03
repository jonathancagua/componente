/*
 * bme280_def.h
 *
 *  Created on: Sep 26, 2021
 *      Author: jonathancagua
 */

#ifndef BME280_INC_BME28_H_
#define BME280_INC_BME28_DEF_H_

#include "bme280_def.h"

int8_t bme280_init(struct bme280_dev *dev);
int8_t bme280_config_set(struct bme280_dev *dev, struct bme280_enable *config);
int8_t bme280_mode_set(struct bme280_dev *dev, uint8_t sensor_mode);
#endif /* BME280_INC_BME28_DEF_H_ */
