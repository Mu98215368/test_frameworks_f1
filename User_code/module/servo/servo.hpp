#pragma once
#include "bsp/error/error.hpp"
#include "bsp/pwm/pwm.hpp"

namespace module
{
class Servo
{
public:
    Servo(bsp::Pwm* pwm, uint16_t arr);
    bsp::ErrorCode init();
    // angle: 0 ~ 180.0f
    bsp::ErrorCode set_angle(float angle);
private:
    bsp::Pwm* m_pwm;
    uint16_t m_arr;
    static constexpr float MIN_ANGLE = 0.0f;
    static constexpr float MAX_ANGLE = 180.0f;
    // 50Hz: 20ms周期，0.5ms ~ 2.5ms
    static constexpr float MIN_DUTY_RATIO = 0.5f / 20.0f;
    static constexpr float MAX_DUTY_RATIO = 2.5f / 20.0f;
};
}
