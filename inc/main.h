/**
 * @file main.h
 * @author jonathan.cagua@gmail.com
 * @brief main. para poner ciertas deficiones
 * @version 0.1
 * @date 2021-10-08
 * 
 * @copyright Copyright (c) 2021
 * 
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
#define TICKRATE_HZ (1000)	/* 1000 ticks per second */
/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

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
