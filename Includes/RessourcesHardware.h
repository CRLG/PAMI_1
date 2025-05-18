/*! \file RessoucesHardware.h
	\brief Déclare les ressources hardware du MBED utilisées par le reste du logiciel
*/
#ifndef _RESSOURCES_HARDWARE_H_ 
#define _RESSOURCES_HARDWARE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define I2C_HDL_ELECTROBOT hi2c2
#define I2C_DEFAULT_TIMEOUT (100)

void setLedBuiltin(bool state);
void toggleLedBuiltin();
void CdeMoteur1(float cde_pourcent);
void CdeMoteur2(float cde_pourcent);
void CdeServo1(int ppm1500);
void CdeServo2(int ppm1500);
void CdeServo3(int ppm1500);
short getCodeur1();
short getCodeur2();
unsigned int readAnalog(int channel);
float readAnalogVolt(int channel);

#ifdef __cplusplus
}
#endif

#endif


