
#include "sapi.h"
#include "main.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define TICKRATE_HZ (1000)	/* 1000 ticks per second */

static void Inicio (void);
void atenderError();
int main(void){
	Inicio();

	while (1)
	{
		__WFI();
	}

	atenderError();
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





