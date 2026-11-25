#pragma once
#include "bsp/error/error.hpp"
#include "bsp/pwm/pwm.hpp"
#include "bsp/gpio/gpio.hpp"
namespace module
{
class Motor
{
public:
    Motor(bsp::Pwm* pwm, bsp::Gpio* in1, bsp::Gpio* in2);
    bsp::ErrorCode init();
    bsp::ErrorCode forward(uint16_t duty);
    bsp::ErrorCode backward(uint16_t duty);
    bsp::ErrorCode stop();
private:
    bsp::Pwm* m_pwm;
    bsp::Gpio* m_in1;
    bsp::Gpio* m_in2;
};
}
