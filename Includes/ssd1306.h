#ifndef _SSD1306_H_
#define _SSD1306_H_

#include "ssd1306_base.h"

class SSD1306 : public SSD1306Base
{
public :
    SSD1306();

    // Méthode virtuelle réimplémentées sur la plateforme
    /*virtual*/bool I2C_Write(uint8_t i2c_addr, uint8_t *pData, uint16_t Size, uint32_t Timeout);
    /*virtual*/bool isPresent(uint8_t i2c_addr);
};

#endif
