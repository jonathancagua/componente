/**
 * @file bme280_port.h
 * @author jonathan.cagua@gmail.com
 * @brief contiene las definiciones de las funciones donde va a saltar las funciones de bme280.*
 * @version 0.1
 * @date 2021-10-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef INC_BME280_PORT_H_
#define INC_BME280_PORT_H_
#include <stdint.h>
#include <stdbool.h>
/**
 * @brief para manejar el tipo de disposisitvo
 * 
 */
struct identifier
{
    uint8_t devi_addr;
    int8_t fd;
};
/**
 * @brief prototipo donde se debe poner las funciones de i2c de sus dispisitivo
 * 
 * @param reg_addr direccion del registro a leer
 * @param reg_data buffer donde se va almacenar la informaciond el sensor por i2c
 * @param len longitud del data a recibir 
 * @param ptrInt puntero del tipo de interfaz
 * @return int8_t retorna algun error
 */
int8_t bme280RegistersRead(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *ptrInt);
/**
 * @brief 
 * 
 * @param reg_addr registro a escribir
 * @param reg_data puntero del buffer a enviar
 * @param len longitud del data a enviar por i2c
 * @param ptrInt puntero del tipo de interfaz
 * @return int8_t 
 */
int8_t bme280RegisterWrite(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *ptrInt);
/**
 * @brief esta funcion se debe agregar el delay dependiendo de sus arquitectura
 * 
 * @param ms valor de milisegundo que se necesita ejecutar
 * @param intf_ptr puntero del tipo de interfaz
 */
void user_delay_ms(uint32_t ms, void *intf_ptr);
#endif /* INC_BME280_PORT_H_ */
