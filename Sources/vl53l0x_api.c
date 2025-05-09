#include "vl53l0x_api.h"

#define VL53L0X_REG_SYSRANGE_START   0x00
#define VL53L0X_REG_RESULT_RANGE     0x1E
#define VL53L0X_REG_INTERRUPT_STATUS 0x13
#define VL53L0X_REG_CLEAR_INTERRUPT  0x0B

static I2C_HandleTypeDef* vl53_i2c;

void VL53L0X_Init(I2C_HandleTypeDef* hi2c) {
    vl53_i2c = hi2c;
    uint8_t data = 0x01;
    HAL_I2C_Mem_Write(vl53_i2c, VL53L0X_I2C_ADDR, VL53L0X_REG_START, 1, &data, 1, HAL_MAX_DELAY);
    HAL_Delay(2);
}

uint16_t VL53L0X_ReadDistanceMM() {
    uint8_t buf[2];
    HAL_I2C_Mem_Read(vl53_i2c, VL53L0X_I2C_ADDR, VL53L0X_REG_RESULT, 1, buf, 2, HAL_MAX_DELAY);
    return (buf[0] << 8) | buf[1];
}

void VL53L0X_StartContinuous(I2C_HandleTypeDef* hi2c) {
    vl53_i2c = hi2c;
    uint8_t start = 0x02;
    HAL_I2C_Mem_Write(vl53_i2c, VL53L0X_I2C_ADDR, VL53L0X_REG_SYSRANGE_START, 1, &start, 1, HAL_MAX_DELAY);
    HAL_Delay(10);
}

uint16_t VL53L0X_ReadRangeContinuousMM() {
    uint8_t status;
    do {
        HAL_I2C_Mem_Read(vl53_i2c, VL53L0X_I2C_ADDR, VL53L0X_REG_INTERRUPT_STATUS, 1, &status, 1, HAL_MAX_DELAY);
    } while ((status & 0x07) == 0);

    uint8_t buf[2];
    HAL_I2C_Mem_Read(vl53_i2c, VL53L0X_I2C_ADDR, VL53L0X_REG_RESULT_RANGE, 1, buf, 2, HAL_MAX_DELAY);
    uint16_t distance = (buf[0] << 8) | buf[1];

    uint8_t clear = 0x01;
    HAL_I2C_Mem_Write(vl53_i2c, VL53L0X_I2C_ADDR, VL53L0X_REG_CLEAR_INTERRUPT, 1, &clear, 1, HAL_MAX_DELAY);

    return distance;
}

void VL53L0X_StopContinuous() {
    uint8_t stop = 0x00;
    HAL_I2C_Mem_Write(vl53_i2c, VL53L0X_I2C_ADDR, VL53L0X_REG_SYSRANGE_START, 1, &stop, 1, HAL_MAX_DELAY);
}
