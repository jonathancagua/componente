/**
 * @file bme_core.h
 * @author jonathan.cagua@gmail.com
 * @brief funciones que se va usar en el app principal 
 * @version 0.1
 * @date 2021-10-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef INC_BME_CORE_H_
#define INC_BME_CORE_H_
/**
 * @brief enum para setear el modo a trabajar
 * 
 */
enum bme_mode
{
    BME_MODE_FORCED = 1,
    BME_MODE_SLEEP,
	BME_MODE_NORMAL
};
/**
 * @brief estrucutura para obetener el data del sensor
 * 
 */
struct bme_data
{
    double pressure;
    double temperature;
    double humidity;
};
/**
 * @brief funcion para inicilizar el bme280
 * 
 * @return int8_t lista de errores
 */
int8_t bme_sensor_init();
/**
 * @brief funcion para optener el dato procesado y se pone la opcion del modo
 * 
 * @param mode modo de lectura
 * @param dev_info va llevar la informacion del device.
 * @return int8_t 
 */
int8_t bme_sensor_get(enum bme_mode mode ,struct bme_data *dev_info);
#endif /* INC_BME_CORE_H_ */
