#include "bsp/pwm/pwm.hpp"
namespace bsp
{
Pwm::Pwm(TIM_HandleTypeDef* htim, uint32_t ch)
    : m_htim(htim), m_ch(ch)
{}

ErrorCode Pwm::start()
{
    if(HAL_TIM_PWM_Start(m_htim, m_ch) != HAL_OK)
    {
        return ErrorCode::INIT_FAIL;
    }
    return ErrorCode::OK;
}

ErrorCode Pwm::stop()
{
    HAL_TIM_PWM_Stop(m_htim, m_ch);
    return ErrorCode::OK;
}

ErrorCode Pwm::set_duty(uint16_t duty)
{
    __HAL_TIM_SET_COMPARE(m_htim, m_ch, duty);
    return ErrorCode::OK;
}
}
