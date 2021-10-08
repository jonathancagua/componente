/*
 * bme_core.h
 *
 *  Created on: Oct 8, 2021
 *      Author: jonathancagua
 */

#ifndef INC_BME_CORE_H_
#define INC_BME_CORE_H_

enum bme_mode
{
    BME_MODE_FORCED = 1,
    BME_MODE_SLEEP,
	BME_MODE_NORMAL
};
struct bme_data
{
    double pressure;
    double temperature;
    double humidity;
};
int8_t bme_sensor_init();
int8_t bme_sensor_get(enum bme_mode mode ,struct bme_data *dev_info);
#endif /* INC_BME_CORE_H_ */
