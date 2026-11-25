#pragma once
#include "bsp/error/error.hpp"
#include "stm32f1xx_hal.h"
namespace bsp
{
class Gpio
{
public:
    Gpio(GPIO_TypeDef* port, uint16_t pin);
    void set_high();
    void set_low();
    void toggle();
    bool read();
private:
    GPIO_TypeDef* m_port;
    uint16_t m_pin;
};
}
