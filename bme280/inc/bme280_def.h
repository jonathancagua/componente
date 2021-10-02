/*
 * bme28_def.h
 *
 *  Created on: Sep 26, 2021
 *      Author: jonathancagua
 */

#ifndef BME280_INC_BME280_DEF_H_
#define BME280_INC_BME280_DEF_H_
#include <stdint.h>
#include <stdbool.h>
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
#define BME280_LEN_CALIB_HUM                    0X07U

#define BME280_NOT_O_SAMPLE                     0x00U
#define BME280_OSAMPLE_1X                       0x01U
#define BME280_OSAMPLE_2X                       0x02U
#define BME280_OSAMPLE_4X                       0x03U
#define BME280_OSAMPLE_8X                       0x04U
#define BME280_OSAMPLE_16X                      0x05U

#define BME280_FIL_COE_OFF                      0x00U
#define BME280_FIL_COE_2                        0x01U
#define BME280_FIL_COE_4                        0x02U
#define BME280_FIL_COE_8                        0x03U
#define BME280_FIL_COE_16                       0x04U

#define BME280_MASK_C_MEAS_T                    0XE0U
#define BME280_MASK_C_MEAS_P                    0X1CU

#define BME280_POWER_SLEEP                      0X00U
#define BME280_POWER_FORCED                     0X01U
#define BME280_POWER_NORMAL                     0X03U

#define BME280_INTF_RESP_SUCCESS                (0)

#define ERROR_NOT                               (0)
#define ERROR_PTR_NULL                          (-1)
#define ERROR_LOGIN_FAIL                        (-2)
#define ERROR_NVM_NOT_COPY                      (-3)
#define ERROR_DEV_NOT_IDENT                     (-4)

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
struct bme280_enable
{
    //oversampling
    bool temp;
    bool hum;
    bool pres;
    bool filter;
};
typedef struct 
{
    //oversampling
    uint8_t osrs_p;
    uint8_t osrs_t;
    uint8_t osrs_h;
    //filter
    uint8_t filter;
}bme280_config;
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
    bme280_config config;
};
#endif /* BME280_INC_BME280_DEF_H_ */
