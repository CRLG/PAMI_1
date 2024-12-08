/*! \file RessoucesHardware.cpp
	\brief Déclare les ressources hardware du MBED utilisées par le reste du logiciel
*/
#include "RessourcesHardware.h"


void setLedBuiltin(bool state)
{
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, (GPIO_PinState)state);
}

void toggleLedBuiltin()
{
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}
