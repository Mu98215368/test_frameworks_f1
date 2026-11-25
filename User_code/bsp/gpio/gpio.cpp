#include "bsp/gpio/gpio.hpp"

namespace bsp
{
Gpio::Gpio(GPIO_TypeDef* port, uint16_t pin)
    : m_port(port), m_pin(pin)
{

}

void Gpio::set_high()
{
    HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_SET);
}

void Gpio::set_low()
{
    HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_RESET);
}

void Gpio::toggle()
{
    HAL_GPIO_TogglePin(m_port, m_pin);
}

bool Gpio::read()
{
    return HAL_GPIO_ReadPin(m_port, m_pin) == GPIO_PIN_SET;
}
}
