
#include "sapi.h"
#include "main.h"
#include "../bme280/inc/bme280.h"
/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define TICKRATE_HZ (1000)	/* 1000 ticks per second */
struct identifier
{
    uint8_t devi_addr;
    int8_t fd;
};
static void Inicio (void);
void atenderError();
static int8_t bme280ReadRegisters(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *ptrInt);
static int8_t bme280WriteRegister(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *ptrInt);
int main(void){
	struct identifier id;
	struct bme280_dev device;
	Inicio();
	id.devi_addr = BME280_I2C_ADDR_1;
	id.fd = i2cInit( I2C0, 100000 );

	device.inter = BME280_INTF_I2C;
	device.fread = bme280ReadRegisters;
	device.fwrite = bme280WriteRegister;
	device.ptrInt = &id;
	delay( 1000 );
	bme280_init(&device);


	while (1)
	{
		__WFI();
	}

	atenderError();
}
static int8_t bme280WriteRegister(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *ptrInt)
{
//	uint8_t transmitDataBuffer[2];
//	transmitDataBuffer[0] = subAddress;
//	transmitDataBuffer[1] = data;
//	i2cWrite(I2C0, control.address, transmitDataBuffer, 2, TRUE);
//
//	delay(10);
//
//	/* read back the register */
//	mpu60X0ReadRegisters(subAddress,1);
//	/* check the read back register against the written register */
//	if(control._buffer[0] == data) {
//      return 1;
//	}
//	else{
//      return -1;
//	}
}

static int8_t bme280ReadRegisters(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *ptrInt)
{
	struct identifier id;

    id = *((struct identifier *)ptrInt);

	if( i2cRead( I2C0,id.devi_addr,&reg_addr,1,TRUE,reg_data,len,TRUE) ){
		return 0;
	} else {
		return -1;
	}
}
// Inicia soporte de la placa y periodo de la interrupcion del SYSTICK
// cada 1 milisegundo.
static void Inicio (void)
{
	SystemCoreClockUpdate();
	Board_Init();
	boardInit();
	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);

}


// No se implementa la atención del error, se deja colgado con el while(1)
void atenderError()
{
	while(1);
}





