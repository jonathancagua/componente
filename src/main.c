/**
 * @file main.c
 * @author jonathan.cagua@gmail.com
 * @brief pequeno main para el uso del componente con placa de LPCxpresso4367 y sapi 
 * @version 0.1
 * @date 2021-10-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "sapi.h"
#include "main.h"
#include "../bme280/inc/bme_core.h"

static void HW_init(void);
void atenderError();
/**
 * @brief This function starts execution of the program.
 * 
 * @return int 
 */
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
/**
 * @brief se inicializa el hardware a usar
 * 
 */
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

/**
 * @brief loop para manejo de errores.
 * 
 */
void atenderError()
{
	while(1)
	{
		__WFI();
	}
}





