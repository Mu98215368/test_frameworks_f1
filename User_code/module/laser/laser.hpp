#pragma once
#include "bsp/error/error.hpp"
#include "bsp/gpio/gpio.hpp"

namespace module
{
class Laser
{
public:
    Laser(bsp::Gpio* gpio);
    bsp::ErrorCode init();    // 加上这个
    void on();
    void off();
private:
    bsp::Gpio* m_gpio;
};
}
