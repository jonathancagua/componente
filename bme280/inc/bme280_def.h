/*
 * bme28_def.h
 *
 *  Created on: Sep 26, 2021
 *      Author: jonathancagua
 */

#ifndef BME280_INC_BME280_DEF_H_
#define BME280_INC_BME280_DEF_H_
#include <stdint.h>
//Direcciones I2C
#define BME280_I2C_ADDR_1                       0x76U
#define BME280_I2C_ADDR_2                       0x77U

//Identificador
#define BME280_CHIP_ID                          0x60U

//Register Description
#define BME280_ADDR_CHIP_ID                     0xD0U
#define BME280_ADDR_RESET                       0xE0U
#define BME280_ADDR_CTRL_HUM                    0xF2U
#define BME280_ADDR_STATUS                      0xF3U
#define BME280_ADDR_CTRL_MEAS                   0xF4U
#define BME280_ADDR_CONFIG                      0xF5U
#define BME280_ADDR_DATA                        0xF7U

#define BME280_RESET_PW_ON                      0xB6U
#define BME280_STATUS_IM_UPDATE                 0x01U

#define BME280_INTF_RESP_SUCCESS                (0)

#define ERROR_NOT                               (0)
#define ERROR_PTR_NULL                          (-1)
#define ERROR_LOGIN_FAIL                        (-2)
#define ERROR_NVM_NOT_COPY                      (-3)
enum bme280_inter
{
    BME280_INTF_I2C = 1,
    BME280_INTF_SPI
};

typedef int8_t (*bme280_func_ptr_t)(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *ptrInt);
typedef void (*bme280_func_delay_ptr_t)(uint32_t ms, void *intf_ptr);
struct bme280_dev
{
	uint8_t chip_id;
    void *ptrInt;
    enum bme280_inter inter;
    bme280_func_ptr_t fwrite;
    bme280_func_ptr_t fread;
    bme280_func_delay_ptr_t delay_ms;
    int8_t inter_resp;
};
#endif /* BME280_INC_BME280_DEF_H_ */
