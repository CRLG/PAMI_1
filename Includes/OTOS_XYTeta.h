#pragma once

#include "OTOS_XYTeta_Base.h"


class OtosXYTeta : public OtosXYTetaBase
{
  public:
    /// @brief Default constructor, only initializes member variables
	OtosXYTeta();

  protected:
    // Virtual function that must be implemented by the derived class to delay
    // for a given number of milliseconds
    /*virtual*/ void delayMs(uint32_t ms) override;
    /*virtual*/ otos_error_t writeRegister(uint8_t reg, uint8_t *buff, uint16_t size) override;
    /*virtual*/ otos_error_t readRegister(uint8_t reg, uint8_t *buff, uint16_t size, uint16_t &byte_read) override;

    uint8_t m_i2c_addr;



};
