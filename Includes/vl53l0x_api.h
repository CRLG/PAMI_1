#ifndef VL53L0X_API_H
#define VL53L0X_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"  // Adapter selon votre série STM32




#define VL53L0X_I2C_ADDR       (0x29 << 1)
#define VL53L0X_REG_RESULT     0x1E
#define VL53L0X_REG_START      0x00
#define VL53L0X_REG_STATUS     0x14

void VL53L0X_Init(I2C_HandleTypeDef* hi2c);
uint16_t VL53L0X_ReadDistanceMM();

void VL53L0X_StartContinuous(I2C_HandleTypeDef* hi2c);
uint16_t VL53L0X_ReadRangeContinuousMM();
void VL53L0X_StopContinuous();

#ifdef __cplusplus
}
#endif

#endif
