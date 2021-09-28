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

#define BME280_ADDR_CALIB_TEMP                  0X88U
#define BME280_ADDR_CALIB_HUME                  0XE1U

#define BME280_LEN_CALIB_TEMP_HUM               0X1AU

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
//table 16
typedef struct __attribute__((__packed__)) 
{
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;

    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;

    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t dig_H6;
    int32_t t_coef;
}dev_calib_data;
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
    dev_calib_data calib_data;
};
#endif /* BME280_INC_BME280_DEF_H_ */
