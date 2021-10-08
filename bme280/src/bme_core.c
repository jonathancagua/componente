/**
 * @file bme_core.c
 * @author jonathan.cagua@gmail.com
 * @brief este .c se encarga de unir bme280_port.* con bme280.* y hacer un solicitud mas sencilla del data 
 * del sensor.
 * @version 0.1
 * @date 2021-10-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bme280.h"
#include "bme280_port.h"
#include "bme_core.h"
static struct bme280_dev device;

int8_t bme_sensor_init(){
	int8_t resp = ERROR_NOT;
	struct identifier id;
	id.devi_addr = BME280_I2C_ADDR_1;
	id.fd = true;

	device.inter = BME280_INTF_I2C;
	device.fread = bme280RegistersRead;
	device.fwrite = bme280RegisterWrite;
	device.delay_ms = user_delay_ms;
	device.ptrInt = &id;
	resp = bme280_init(&device);
	if(resp == ERROR_NOT){
		struct bme280_enable sensor_select;

		sensor_select.hum = true;
		sensor_select.pres = true;
		sensor_select.temp = true;
		sensor_select.filter = true;
		device.config.osrs_t = BME280_OSAMPLE_2X;
		device.config.osrs_h = BME280_OSAMPLE_1X;
		device.config.osrs_p = BME280_OSAMPLE_16X;
		device.config.filter = BME280_FIL_COE_16;
		resp = bme280_config_set(&device, &sensor_select);
		if (resp != ERROR_NOT)
	    {
	        return resp;
	    }
	}
	return(resp);
}

int8_t bme_sensor_get(enum bme_mode mode ,struct bme_data *dev_info){
	int8_t resp = ERROR_NOT;
	struct bme280_data dev_data;
	if( BME_MODE_FORCED == mode){
		resp = bme280_mode_set(&device, BME280_POWER_FORCED);
		if (resp != ERROR_NOT)
		{
			return resp;
		}
		device.delay_ms(40, device.ptrInt);
		resp = bme280_data_get(&device, &dev_data);
		if(resp == ERROR_NOT){
			memcpy(dev_info,&dev_data,sizeof(struct bme_data));
		}
		else{
			memset(dev_info,0xFF,sizeof(struct bme_data));
		}
	}
	return(resp);
}
