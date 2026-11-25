#include "bsp/adc/adc.hpp"

namespace bsp
{
Adc::Adc(ADC_HandleTypeDef* hadc, Mode mode)
    : m_hadc(hadc), m_mode(mode)
{

}

ErrorCode Adc::start()
{
    if(m_mode == BLOCK)
    {
        if(HAL_ADC_Start(m_hadc) != HAL_OK)
        {
            return ErrorCode::BUS_BUSY;
        }
    }
    else if(m_mode == DMA)
    {
        if(HAL_ADC_Start_DMA(m_hadc, nullptr, 0) != HAL_OK)
        {
            return ErrorCode::BUS_BUSY;
        }
    }
    return ErrorCode::OK;
}

ErrorCode Adc::stop()
{
    if(m_mode == BLOCK)
    {
        if(HAL_ADC_Stop(m_hadc) != HAL_OK)
        {
            return ErrorCode::BUS_BUSY;
        }
    }
    else if(m_mode == DMA)
    {
        if(HAL_ADC_Stop_DMA(m_hadc) != HAL_OK)
        {
            return ErrorCode::BUS_BUSY;
        }
    }
    return ErrorCode::OK;
}

ErrorCode Adc::read(uint16_t* val)
{
    if(m_mode == BLOCK)
    {
        if(HAL_ADC_PollForConversion(m_hadc, 100) != HAL_OK)
        {
            return ErrorCode::TIMEOUT;
        }
        *val = HAL_ADC_GetValue(m_hadc);
    }
    else
    {
        return ErrorCode::ERROR;
    }
    return ErrorCode::OK;
}
}
