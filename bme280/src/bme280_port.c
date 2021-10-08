/*
 * bme280_core.c
 *
 *  Created on: Oct 8, 2021
 *      Author: jonathancagua
 */

#include "bme280_port.h"
#include "bme280.h"
//Incluir las librerias de su microcontolador
#include "sapi.h" //libreias de la CIAA
//-------------------------------------------

int8_t bme280RegisterWrite(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *ptrInt)
{

	struct identifier id;
    id = *((struct identifier *)ptrInt);
    uint8_t *buff = malloc((1 + len) * sizeof(uint8_t));
    buff[0] = reg_addr;
    for (uint8_t i = 0; i < len; i++)
    {
        buff[i + 1] = reg_data[i];
    }
    //--------------AGREGAR LA FUNCION DE I2C DE SU MICROCONTROLADOR-------
	i2cWrite( I2C0,id.devi_addr,buff,len+1,TRUE);
	//----------------------------------------------------------------------
	free(buff);
	return 0;
}

int8_t bme280RegistersRead(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *ptrInt)
{
	struct identifier id;
    id = *((struct identifier *)ptrInt);
    //--------------AGREGAR LA FUNCION DE I2C DE SU MICROCONTROLADOR-------
	if( i2cRead( I2C0,id.devi_addr,&reg_addr,1,TRUE,reg_data,len,TRUE) ){
	//----------------------------------------------------------------------
		return 0;
	} else {
		return -1;
	}
}

void user_delay_ms(uint32_t ms, void *intf_ptr)
{
	//--------------AGREGAR LA FUNCION DE I2C DE SU MICROCONTROLADOR-------
    delay( ms );
	//----------------------------------------------------------------------
}
