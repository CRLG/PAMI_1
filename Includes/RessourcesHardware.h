/*! \file RessoucesHardware.h
	\brief Déclare les ressources hardware du MBED utilisées par le reste du logiciel
*/
#ifndef _RESSOURCES_HARDWARE_H_ 
#define _RESSOURCES_HARDWARE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void setLedBuiltin(bool state);
void toggleLedBuiltin();

#ifdef __cplusplus
}
#endif

#endif


