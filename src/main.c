
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
static int8_t bme280RegistersRead(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *ptrInt);
static int8_t bme280RegisterWrite(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *ptrInt);
void user_delay_ms(uint32_t ms, void *intf_ptr);
int8_t script_mode_force(struct bme280_dev *device){
	int8_t resp = ERROR_NOT;
	struct bme280_enable sensor_select;

	sensor_select.hum = TRUE;
	sensor_select.pres = TRUE;
	sensor_select.temp = TRUE;
	sensor_select.filter = TRUE;
	device->config.osrs_t = BME280_OSAMPLE_2X;
	device->config.osrs_h = BME280_OSAMPLE_1X;
	device->config.osrs_p = BME280_OSAMPLE_16X;
	device->config.filter = BME280_FIL_COE_16;
	resp = bme280_config_set(device, &sensor_select);
	if (resp != ERROR_NOT)
    {
		printf(stderr, "Falla en algun paso de la configuracion \n\r");

        return resp;
    }
	return(resp);
	
}
int main(void){
	int8_t resp = ERROR_NOT;
	struct identifier id;
	struct bme280_dev device;
	Inicio();
	id.devi_addr = BME280_I2C_ADDR_1;
	id.fd = i2cInit( I2C0, 100000 );

	device.inter = BME280_INTF_I2C;
	device.fread = bme280RegistersRead;
	device.fwrite = bme280RegisterWrite;
	device.delay_ms = user_delay_ms;
	device.ptrInt = &id;
	delay( 1000 );
	resp = bme280_init(&device);
	if(resp != ERROR_NOT){
		printf("Error para inicializar \n\r");
	}
	script_mode_force(&device);
	while (1)
	{
		__WFI();
	}

	atenderError();
}
void user_delay_ms(uint32_t ms, void *intf_ptr)
{
    delay( ms );
}
static int8_t bme280RegisterWrite(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *ptrInt)
{
	struct identifier id;
    id = *((struct identifier *)ptrInt);
	i2cWrite( I2C0,id.devi_addr,reg_data,len,TRUE);
	return 0;
}

static int8_t bme280RegistersRead(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *ptrInt)
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





