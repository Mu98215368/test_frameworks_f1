#include "bsp/uart/uart.hpp"

namespace bsp
{
Uart* Uart::g_instance = nullptr;

Uart::Uart(UART_HandleTypeDef* huart)
    : m_huart(huart)
{
    g_instance = this;
}

ErrorCode Uart::init(uint8_t addr, TransMode mode)
{
    (void)addr;
    m_mode = mode;
    if (m_huart == nullptr)
    {
        return ErrorCode::INVALID_ARG;
    }
    return ErrorCode::OK;
}

ErrorCode Uart::set_callback(DevTransCallback cb, void* user_arg)
{
    m_cb = cb;
    m_user_arg = user_arg;
    return ErrorCode::OK;
}

ErrorCode Uart::write(const uint8_t* buf, uint16_t len)
{
    if (buf == nullptr || len == 0)
    {
        return ErrorCode::INVALID_ARG;
    }

    uint8_t* p = const_cast<uint8_t*>(buf);

    if (m_mode == TransMode::BLOCK)
    {
        if (HAL_UART_Transmit(m_huart, buf, len, 100) != HAL_OK)
        {
            return ErrorCode::BUS_BUSY;
        }
        return ErrorCode::OK;
    }
    else if (m_mode == TransMode::IT)
    {
        if (HAL_UART_Transmit_IT(m_huart, p, len) != HAL_OK)
        {
            return ErrorCode::BUS_BUSY;
        }
        return ErrorCode::OK;
    }
    return ErrorCode::INVALID_ARG;
}

ErrorCode Uart::read(uint8_t* buf, uint16_t len, uint16_t* read_len)
{
    if (buf == nullptr || read_len == nullptr || len == 0)
    {
        return ErrorCode::INVALID_ARG;
    }
    *read_len = 0;

    if (m_mode == TransMode::BLOCK)
    {
        if (HAL_UART_Receive(m_huart, buf, len, 100) != HAL_OK)
        {
            return ErrorCode::TIMEOUT;
        }
        *read_len = len;
        return ErrorCode::OK;
    }
    else if (m_mode == TransMode::IT)
    {
        if (HAL_UART_Receive_IT(m_huart, buf, len) != HAL_OK)
        {
            return ErrorCode::BUS_BUSY;
        }
        return ErrorCode::OK;
    }
    return ErrorCode::INVALID_ARG;
}

ErrorCode Uart::control(int32_t cmd, void* arg)
{
    switch (cmd)
    {
        case DEV_CTRL_SET_MODE:
            m_mode = *static_cast<TransMode*>(arg);
            return ErrorCode::OK;
        case DEV_CTRL_RESET:
            if (HAL_UART_DeInit(m_huart) != HAL_OK)
            {
                return ErrorCode::INIT_FAIL;
            }
            if (HAL_UART_Init(m_huart) != HAL_OK)
            {
                return ErrorCode::INIT_FAIL;
            }
            return ErrorCode::OK;
        case DEV_CTRL_SET_BAUD:
        {
            uint32_t baud = *static_cast<uint32_t*>(arg);
            m_huart->Init.BaudRate = baud;
            if (HAL_UART_Init(m_huart) != HAL_OK)
            {
                return ErrorCode::INIT_FAIL;
            }
            return ErrorCode::OK;
        }
        default:
            return ErrorCode::INVALID_ARG;
    }
}

void Uart::tx_wrapper(UART_HandleTypeDef* huart)
{
    (void)huart;
    if (g_instance != nullptr && g_instance->m_cb != nullptr)
    {
        g_instance->m_cb(g_instance->m_user_arg);
    }
}

void Uart::rx_wrapper(UART_HandleTypeDef* huart)
{
    (void)huart;
    if (g_instance != nullptr && g_instance->m_cb != nullptr)
    {
        g_instance->m_cb(g_instance->m_user_arg);
    }
}
}