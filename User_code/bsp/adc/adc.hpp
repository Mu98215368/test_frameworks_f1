#pragma once
#include "bsp/error/error.hpp"
#include "stm32f1xx_hal.h"
#include <cstdint>

namespace bsp
{
class Adc
{
public:
    enum Mode
    {
        BLOCK,
        DMA
    };
    Adc(ADC_HandleTypeDef* hadc, Mode mode);
    ErrorCode start();
    ErrorCode stop();
    ErrorCode read(uint16_t* val);

private:
    ADC_HandleTypeDef* m_hadc;
    Mode m_mode;
};
}
