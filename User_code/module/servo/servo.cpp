#include "servo.hpp"
#include "bsp/log/log.hpp"
namespace module
{
Servo::Servo(bsp::Pwm* pwm, uint16_t arr)
    : m_pwm(pwm), m_arr(arr)
{}

bsp::ErrorCode Servo::init()
{
    return m_pwm->start();
}

bsp::ErrorCode Servo::set_angle(float angle)
{
    // 限幅
    if (angle < MIN_ANGLE) angle = MIN_ANGLE;
    if (angle > MAX_ANGLE) angle = MAX_ANGLE;

    // 角度映射到占空比比例
    float ratio = MIN_DUTY_RATIO + (MAX_DUTY_RATIO - MIN_DUTY_RATIO) * (angle / MAX_ANGLE);
    uint16_t duty = static_cast<uint16_t>(ratio * m_arr);

    auto ret = m_pwm->set_duty(duty);
    if (ret != bsp::ErrorCode::OK)
    {
        bsp::log_error(ret, "Servo set angle fail\r\n");
    }
    return ret;
}
}
