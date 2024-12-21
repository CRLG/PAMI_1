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
void CdeMoteur1(float cde_pourcent);
void CdeMoteur2(float cde_pourcent);
void CdeServo1(int ppm1500);
void CdeServo2(int ppm1500);
void CdeServo3(int ppm1500);

int getCodeur1();
int getCodeur2();
int readAnalog(int channel);

#ifdef __cplusplus
}
#endif

#endif


