
/*
 * main.h
 *
 *  Created on: Sep 26, 2021
 *      Author: jonathancagua
 */
/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __MAIN_H__
#define __MAIN_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include <stdint.h>
#include <stddef.h>
/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/
#define LED_OFF	LED3+1

/*=====[Prototypes (declarations) of public functions]=======================*/

/* Funcion atenderError
 * Funcion para atender errores no implementada
 * Se dejo un while(1) para que se quede en esta función en caso de error*/
void atenderError();

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __MAIN_H__ */
