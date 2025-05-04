/**
 * original author:  Tilen Majerle<tilen@majerle.eu>
 * modification for STM32f10x: Alexander Lutsai<s.lyra@ya.ru>

   ----------------------------------------------------------------------
   	Copyright (C) Alexander Lutsai, 2016
    Copyright (C) Tilen Majerle, 2015

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
 */
#include "ssd1306.h"
#include "RessourcesHardware.h"

SSD1306::SSD1306()
{
}

// _____________________________________________
bool SSD1306::I2C_Write(uint8_t i2c_addr, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    return (HAL_I2C_Master_Transmit(&I2C_HDL_ELECTROBOT, i2c_addr, pData, Size, Timeout)==HAL_OK);
}

// _____________________________________________
bool SSD1306::isPresent(uint8_t i2c_addr)
{
    uint8_t data;
    return ( HAL_I2C_Master_Transmit(&I2C_HDL_ELECTROBOT, i2c_addr, &data, 1, I2C_DEFAULT_TIMEOUT) == HAL_OK );
}

