#include "motor.hpp"
#include "bsp/log/log.hpp"
namespace module
{
Motor::Motor(bsp::Pwm* pwm, bsp::Gpio* in1, bsp::Gpio* in2)
    : m_pwm(pwm), m_in1(in1), m_in2(in2)
{}

bsp::ErrorCode Motor::init()
{
    auto ret = m_pwm->start();
    if(ret != bsp::ErrorCode::OK)
        bsp::log_error(ret, "Motor pwm start fail\r\n");
    return ret;
}

bsp::ErrorCode Motor::forward(uint16_t duty)
{
    m_in1->set_high();
    m_in2->set_low();
    return m_pwm->set_duty(duty);
}

bsp::ErrorCode Motor::backward(uint16_t duty)
{
    m_in1->set_low();
    m_in2->set_high();
    return m_pwm->set_duty(duty);
}

bsp::ErrorCode Motor::stop()
{
    m_in1->set_low();
    m_in2->set_low();
    return m_pwm->set_duty(0);
}
}
