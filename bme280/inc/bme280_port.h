/*
 * bme280_core.h
 *
 *  Created on: Oct 8, 2021
 *      Author: jonathancagua
 */

#ifndef INC_BME280_PORT_H_
#define INC_BME280_PORT_H_
#include <stdint.h>
#include <stdbool.h>

struct identifier
{
    uint8_t devi_addr;
    int8_t fd;
};

int8_t bme280RegistersRead(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *ptrInt);
int8_t bme280RegisterWrite(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *ptrInt);
void user_delay_ms(uint32_t ms, void *intf_ptr);
#endif /* INC_BME280_PORT_H_ */
