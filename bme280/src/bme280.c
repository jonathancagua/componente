#include "bme280.h"
#include <stddef.h>

static int8_t bme280_reg_get(struct bme280_dev *dev, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t resp = ERROR_PTR_NULL;
    if((dev == NULL) && (reg_data == NULL)) return(resp);
    resp = ERROR_NOT;

    dev->inter_resp = dev->fread(reg_addr, reg_data, len, dev->ptrInt);

    if (dev->inter_resp != BME280_INTF_RESP_SUCCESS) resp = ERROR_LOGIN_FAIL;

    return resp;
}
static int8_t bme280_reg_set(struct bme280_dev *dev , uint8_t reg_addr, uint8_t *reg_data, uint8_t len)
{
    int8_t resp = ERROR_PTR_NULL;
    if((dev == NULL)) return(resp);
    resp = ERROR_NOT;

    dev->inter_resp = dev->fwrite(reg_addr, reg_data, len, dev->ptrInt);
    if (dev->inter_resp != BME280_INTF_RESP_SUCCESS) resp = ERROR_LOGIN_FAIL;
}
static int8_t bme280_reset_sw(struct bme280_dev *dev)
{
    uint8_t status_reg = 0;
    uint8_t retry_ = 5;
    uint8_t cmd_rst_procedure = BME280_RESET_PW_ON;
    int8_t resp = ERROR_PTR_NULL;
    if(dev == NULL) return(resp);
    resp = bme280_reg_set(dev, BME280_ADDR_RESET, &cmd_rst_procedure, 1);
    if (resp == ERROR_NOT)
    {
        do
        {
            //Table 1, timer de 2 ms.
            dev->delay_ms(2, dev->ptrInt);
            resp = bme280_reg_get(dev, BME280_ADDR_STATUS, &status_reg, 1 );
        } while ((resp == ERROR_NOT) && (retry_--) && (status_reg & BME280_STATUS_IM_UPDATE));
        if (status_reg & BME280_STATUS_IM_UPDATE) resp = ERROR_NVM_NOT_COPY;
    }
    return(resp);
}
static void bme280_temp_hum_proce(struct bme280_dev *dev, const uint8_t *reg_data)
{
	memset(&dev->calib_data.dig_T1,0x00,sizeof(dev->calib_data));
    memcpy(&dev->calib_data.dig_T1,reg_data,(BME280_LEN_CALIB_TEMP_HUM - 2));
    dev->calib_data.dig_H1 = reg_data[BME280_LEN_CALIB_TEMP_HUM-1];
}
static void bme280_hum_proce(struct bme280_dev *dev, const uint8_t *reg_data)
{
    int16_t dig_lsb;
    int16_t dig_msb;
    memcpy(&dev->calib_data.dig_H2,reg_data,3);
    dig_msb = (int16_t)(int8_t)reg_data[3] * 16;
    dig_lsb = (int16_t)(reg_data[4] & 0x0F);
    dev->calib_data.dig_H4 = dig_msb | dig_lsb;
    dig_msb = (int16_t)(int8_t)reg_data[5] * 16;
    dig_lsb = (int16_t)(reg_data[4] >> 4);
    dev->calib_data.dig_H5 = dig_msb | dig_lsb;
    dev->calib_data.dig_H6 = (int8_t)reg_data[6];
}

static int8_t bme280_calibration_data_get(struct bme280_dev *dev){
    int8_t resp = ERROR_PTR_NULL;
    uint8_t reg_addr = BME280_ADDR_CALIB_TEMP;
    if(dev == NULL) return(resp);
    uint8_t calib_data[BME280_LEN_CALIB_TEMP_HUM]={0};
    resp = bme280_reg_get(dev, reg_addr, calib_data, BME280_LEN_CALIB_TEMP_HUM);
    if(resp == ERROR_NOT){
    	bme280_temp_hum_proce(dev, calib_data );
    	reg_addr = BME280_ADDR_CALIB_HUME;
        resp = bme280_reg_get(dev, reg_addr, calib_data, BME280_LEN_CALIB_TEMP_HUM);
        if(resp == ERROR_NOT){
        	bme280_hum_proce(dev, calib_data );
        }
    }
    return(resp);
}
int8_t bme280_init(struct bme280_dev *dev)
{
    uint8_t chip_id = 0;
    uint8_t retry_ = 5;
    int8_t resp = ERROR_PTR_NULL;
    if(dev == NULL) return(resp);

    while(retry_--){
        resp = bme280_reg_get(dev,BME280_ADDR_CHIP_ID, &chip_id, 1);
        if((resp == ERROR_NOT) && (chip_id == BME280_CHIP_ID)){
            //Se pudo leer bien el id del sensor
            dev->chip_id = chip_id;
            //ahora hacemos un reset de sw
            if(bme280_reset_sw(dev) == ERROR_NOT){
            	resp = bme280_calibration_data_get(dev);
                if(resp == ERROR_NOT){
                    return(resp);
                }
            }
        }
    }
    resp = ERROR_DEV_NOT_IDENT;
    return(resp);

}
static int8_t bme280_mode_get(struct bme280_dev *dev, uint8_t *power_mode){
    int8_t resp = ERROR_PTR_NULL;
    if(dev == NULL || power_mode == NULL) return(resp);
    resp = bme280_reg_get(dev, BME280_ADDR_CTRL_MEAS, power_mode, 1);
    *power_mode &= 0x03;//mode[1:0]    
    return(resp);
}
static int8_t bme280_meas_overwrite(struct bme280_dev *dev){
    uint8_t ctrl_meas_value;
    int8_t resp = ERROR_PTR_NULL;
    if(dev == NULL) return(resp);
    resp = bme280_reg_get(dev, BME280_ADDR_CTRL_MEAS, &ctrl_meas_value, 1);
    if(resp == ERROR_NOT){
        resp = bme280_reg_set(dev,BME280_ADDR_CTRL_MEAS,&ctrl_meas_value,1);
    }
    return(resp);
}

static int8_t bme280_osrs_h_write(struct bme280_dev *dev){
    int8_t resp = ERROR_PTR_NULL;
    if(dev == NULL) return(resp);
    //escribimos humedad
    resp = bme280_reg_set(dev,BME280_ADDR_CTRL_HUM,&dev->config.osrs_h,1);
    if(resp == ERROR_NOT){
        resp = bme280_meas_overwrite(dev);//esto para asegurar escritura humedad
    }
    return(resp);
}
static int8_t bme280_osrs_tp_write(struct bme280_dev *dev, struct bme280_enable *config){
    int8_t resp = ERROR_PTR_NULL;
    uint8_t regis;
    //escribimos temperatura y presion en el mismo registro de 8bits
    if(dev == NULL || config == NULL) return(resp);
    resp = bme280_reg_get(dev, BME280_ADDR_CTRL_MEAS, &regis, 1);
    if(resp == ERROR_NOT){
        if(config->temp){
            regis = regis & (~BME280_MASK_C_MEAS_T);//borro los bit de temperatura en el registro
            regis = regis | ((dev->config.osrs_t << 5) & BME280_MASK_C_MEAS_T);//desplazo los bits
        } 
        if(config->pres){
            regis = regis & (~BME280_MASK_C_MEAS_P);//borro los bit de presion en el registro
            regis = regis | ((dev->config.osrs_p << 2) & BME280_MASK_C_MEAS_P);//desplazo los bits
        } 
        resp = bme280_reg_set(dev,BME280_ADDR_CTRL_MEAS,&regis,1);
    }
    return(resp);
}
static int8_t bme280_config_filter_write(struct bme280_dev *dev, struct bme280_enable *config){
    int8_t resp = ERROR_PTR_NULL;
    uint8_t regis;
    //escribimos opcion filtro en el registro
    if(dev == NULL || config == NULL) return(resp);
    resp = bme280_reg_get(dev, BME280_ADDR_CONFIG, &regis, 1);
    if(resp == ERROR_NOT){
        if(config->filter){
            regis = regis & (~BME280_MASK_CONF_FIL);//borro los bit de filtro en el registro
            regis = regis | ((dev->config.filter << 2) & BME280_MASK_CONF_FIL);//desplazo los bits
        } 
        //TODO poner la opcion de filtro en standby
        resp = bme280_reg_set(dev,BME280_ADDR_CONFIG,&regis,1);
    }
    return(resp);
}
int8_t bme280_config_set(struct bme280_dev *dev, struct bme280_enable *config)
{
    int8_t resp = ERROR_PTR_NULL;
    uint8_t power_mode;
    if(dev == NULL || config == NULL) return(resp);
    resp = bme280_mode_get(dev, &power_mode);
    if((resp = ERROR_NOT) && (power_mode != BME280_POWER_SLEEP)){
        //TODO hacer la opcion de sleep cuando el equipo no esta dormido
    }
    if(resp == ERROR_NOT){
        if(config->hum){
            resp = bme280_osrs_h_write(dev);
        }
        if((resp == ERROR_NOT) && (config->temp || config->pres)){
            resp = bme280_osrs_tp_write(dev, config);
        }
        if((resp == ERROR_NOT) && (config->filter)){
            resp = bme280_config_filter_write(dev, config);
        }
    }
    return(resp);
}
static int8_t bme280_power_set(struct bme280_dev *dev, uint8_t power_modo){
    int8_t resp = ERROR_PTR_NULL;
    uint8_t reg_value;
    if(dev == NULL) return(resp);
    resp = bme280_reg_get(dev, BME280_ADDR_CTRL_MEAS, &reg_value, 1);
    if(resp == ERROR_NOT){
    	reg_value = reg_value & (~BME280_MASK_CONF_PWR);//borro los bit de filtro en el registro
        power_modo = reg_value | (power_modo & BME280_MASK_CONF_PWR);
    	resp = bme280_reg_set(dev,BME280_ADDR_CTRL_MEAS,&power_modo,1);
    }
    return(resp);
}
int8_t bme280_mode_set(struct bme280_dev *dev, uint8_t sensor_mode)
{
	int8_t resp = ERROR_PTR_NULL;
	if(dev == NULL ) return(resp);
    uint8_t mode_now;
	resp = bme280_mode_get(dev, &mode_now);

	if((resp = ERROR_NOT) && (mode_now != BME280_POWER_SLEEP)){
		//TODO hacer la opcion de sleep cuando el equipo no esta dormido
	}
	if(resp == ERROR_NOT){
		bme280_power_set(dev,sensor_mode);
	}
    return resp;
}
double bm280_pressure_convert(struct bme280_dev *dev, struct bme280_data_uncom *dev_data)
{
    double data1,data2,data3;
    double pressure=0;
    data1 = ((double)dev->calib_data.t_coef / 2.0) - 64000.0;
    data2 = data1 * data1 * ((double)dev->calib_data.dig_P6) / 32768.0;
    data2 = data2 + data1 * ((double)dev->calib_data.dig_P5) * 2.0;
    data2 = (data2 / 4.0) + (((double)dev->calib_data.dig_P4) * 65536.0);
    data3 = ((double)dev->calib_data.dig_P3) * data1 * data1 / 524288.0;
    data1 = (data3 + ((double)dev->calib_data.dig_P2) * data1) / 524288.0;
    data1 = (1.0 + data1 / 32768.0) * ((double)dev->calib_data.dig_P1);

    if (data1 > (0.0))
    {
        pressure = 1048576.0 - (double) dev_data->pressure;
        pressure = (pressure - (data2 / 4096.0)) * 6250.0 / data1;
        data1 = ((double)dev->calib_data.dig_P9) * pressure * pressure / 2147483648.0;
        data2 = pressure * ((double)dev->calib_data.dig_P8) / 32768.0;
        pressure = pressure + (data1 + data2 + ((double)dev->calib_data.dig_P7)) / 16.0;
    }
    return(pressure);
}
double bm280_humidity_convert(struct bme280_dev *dev, struct bme280_data_uncom *dev_data)
{
    double data1,data2,data3,data4,data5,data6;
    double humidity;
    data1 = ((double)dev->calib_data.t_coef) - 76800.0;
    data2 = (((double)dev->calib_data.dig_H4) * 64.0 + (((double)dev->calib_data.dig_H5) / 16384.0) * data1);
    data3 = dev_data->humidity - data2;
    data4 = ((double)dev->calib_data.dig_H2) / 65536.0;
    data5 = (1.0 + (((double)dev->calib_data.dig_H3) / 67108864.0) * data1);
    data6 = 1.0 + (((double)dev->calib_data.dig_H6) / 67108864.0) * data1 * data5;
    data6 = data3 * data4 * (data5 * data6);
    humidity = data6 * (1.0 - ((double)dev->calib_data.dig_H1) * data6 / 524288.0);
    return(humidity);
}
double bm280_temperature_convert(struct bme280_dev *dev, struct bme280_data_uncom *dev_data)
{
    double data1,data2;
    double temperature;
	data1 = ((double)dev_data->temperature) / 16384.0 - ((double)dev->calib_data.dig_T1) / 1024.0;
	data1 = data1 * ((double)dev->calib_data.dig_T2);
	data2 = (((double)dev_data->temperature) / 131072.0 - ((double)dev->calib_data.dig_T1) / 8192.0);
	temperature = (data1 + data2) / 5120.0;//test temperature read
    return(temperature);
}

int8_t bme280_data_get(struct bme280_dev *dev, struct bme280_data *dev_data)
{
	int8_t resp = ERROR_PTR_NULL;
	uint8_t data_sensor[8]={0};
    struct bme280_data_uncom data_uncom = {0};
	if(dev == NULL || dev_data == NULL) return(resp);
	resp = bme280_reg_get(dev, BME280_ADDR_DATA, data_sensor, 8);


	data_uncom.pressure = (data_sensor[0]<<12)|(data_sensor[1]<<4)|(data_sensor[2]>>4);
	data_uncom.temperature = (data_sensor[3]<<12)|(data_sensor[4]<<4)|(data_sensor[5]>>4);
	data_uncom.humidity = ((data_sensor[6]<<8)|(data_sensor[7]));
    
    dev_data->temperature = bm280_temperature_convert(dev, &data_uncom);
    dev_data->humidity = bm280_humidity_convert(dev, &data_uncom);
    dev_data->pressure = bm280_pressure_convert(dev, &data_uncom);
	return resp;
}

