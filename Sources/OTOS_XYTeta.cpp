
#include "RessourcesHardware.h"
#include "OTOS_XYTeta.h"

OtosXYTeta::OtosXYTeta()
	: m_i2c_addr(kDefaultAddress<<1)
{
}


void OtosXYTeta::delayMs(uint32_t ms)
{
	HAL_Delay(ms);
}


otos_error_t OtosXYTeta::writeRegister(uint8_t reg, uint8_t *buff, uint16_t size)
{
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&I2C_HDL_ELECTROBOT, m_i2c_addr, reg, 1, buff, size, 100);
	if (status != HAL_OK) return Otos_ErrFail;
	return Otos_OK;
}

otos_error_t OtosXYTeta::readRegister(uint8_t reg, uint8_t *buff, uint16_t size, uint16_t &byte_read)
{
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&I2C_HDL_ELECTROBOT, m_i2c_addr, reg, 1, buff, size, 100);
	if (status != HAL_OK) {
		byte_read = 0;
		return Otos_ErrFail;
	}
	byte_read = size;
	return Otos_OK;
}
