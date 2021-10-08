
#include "sapi.h"
#include "main.h"
#include "../bme280/inc/bme_core.h"

#define TICKRATE_HZ (1000)	/* 1000 ticks per second */

static void HW_init(void);
void atenderError();

int main(void){
	int8_t resp;
	struct bme_data sensor_data;
	HW_init();

	delay( 1000 );
	resp = bme_sensor_init();
	if(resp != 0){
		printf("Error para inicializar \n\r");
	}
	while (resp == 0)
		resp = bme_sensor_get(BME_MODE_FORCED,&sensor_data);

	atenderError();
}

static void HW_init(void)
{
	SystemCoreClockUpdate();
	Board_Init();
	boardInit();
	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);
	/* Enable and setup I2C */
	i2cInit( I2C0, 100000 );

}

// No se implementa la atención del error, se deja colgado con el while(1)
void atenderError()
{
	while(1)
	{
		__WFI();
	}
}





