#pragma once
#include "bsp/error/error.hpp"
#include "stm32f1xx_hal.h"
#include <cstdint>
namespace bsp
{
class Pwm
{
public:
    Pwm(TIM_HandleTypeDef* htim, uint32_t ch);
    ErrorCode start();
    ErrorCode stop();
    ErrorCode set_duty(uint16_t duty);
private:
    TIM_HandleTypeDef* m_htim;
    uint32_t m_ch;
};
}
