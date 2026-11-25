#include "laser.hpp"
namespace module
{
Laser::Laser(bsp::Gpio* gpio)
    : m_gpio(gpio)
{}

bsp::ErrorCode Laser::init()
{
    // 激光GPIO初始化
    m_gpio->set_low();
    return bsp::ErrorCode::OK;
}

void Laser::on()
{
    m_gpio->set_high();
}
void Laser::off()
{
    m_gpio->set_low();
}
}
