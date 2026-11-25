#include "bsp/i2c/i2c.hpp"

namespace bsp
{
I2c::I2c(I2C_HandleTypeDef* hi2c)
    : m_hi2c(hi2c)
{}

ErrorCode I2c::init(uint8_t addr, TransMode mode)
{
    m_addr = addr;
    m_mode = mode;
    if (m_hi2c == nullptr)
    {
        return ErrorCode::INVALID_ARG;
    }
    return ErrorCode::OK;
}

ErrorCode I2c::read(uint8_t* buf, uint16_t len, uint16_t* read_len)
{
    if (buf == nullptr || read_len == nullptr || len == 0U)
    {
        return ErrorCode::INVALID_ARG;
    }
    *read_len = 0U;

    const uint16_t dev_addr = static_cast<uint16_t>(m_addr << 1);

    if (m_mode == TransMode::BLOCK)
    {
        const HAL_StatusTypeDef st = HAL_I2C_Master_Receive(m_hi2c, dev_addr, buf, len, 100);
        if (st == HAL_TIMEOUT)
        {
            return ErrorCode::TIMEOUT;
        }
        if (st == HAL_BUSY)
        {
            return ErrorCode::BUS_BUSY;
        }
        if (st != HAL_OK)
        {
            return ErrorCode::ERROR;
        }
        *read_len = len;
        return ErrorCode::OK;
    }

    // IT 模式：只发起，数据靠回调取
    if (HAL_I2C_Master_Receive_IT(m_hi2c, dev_addr, buf, len) != HAL_OK)
    {
        return ErrorCode::BUS_BUSY;
    }
    return ErrorCode::OK;
}

ErrorCode I2c::write(const uint8_t* buf, uint16_t len)
{
    if (buf == nullptr || len == 0U)
    {
        return ErrorCode::INVALID_ARG;
    }

    uint8_t* p = const_cast<uint8_t*>(buf);
    const uint16_t dev_addr = static_cast<uint16_t>(m_addr << 1);

    if (m_mode == TransMode::BLOCK)
    {
        const HAL_StatusTypeDef st = HAL_I2C_Master_Transmit(m_hi2c, dev_addr, p, len, 100);
        if (st == HAL_TIMEOUT)
        {
            return ErrorCode::TIMEOUT;
        }
        if (st == HAL_BUSY)
        {
            return ErrorCode::BUS_BUSY;
        }
        if (st != HAL_OK)
        {
            return ErrorCode::ERROR;
        }
        return ErrorCode::OK;
    }

    if (HAL_I2C_Master_Transmit_IT(m_hi2c, dev_addr, p, len) != HAL_OK)
    {
        return ErrorCode::BUS_BUSY;
    }
    return ErrorCode::OK;
}

ErrorCode I2c::read_reg(uint8_t reg, uint8_t* buf, uint16_t len)
{
    if (buf == nullptr || len == 0U)
    {
        return ErrorCode::INVALID_ARG;
    }

    const uint16_t dev_addr = static_cast<uint16_t>(m_addr << 1);
    const HAL_StatusTypeDef st = HAL_I2C_Mem_Read(m_hi2c, dev_addr, reg,
                                                  I2C_MEMADD_SIZE_8BIT, buf, len, 100);
    if (st == HAL_TIMEOUT)
    {
        return ErrorCode::TIMEOUT;
    }
    if (st == HAL_BUSY)
    {
        return ErrorCode::BUS_BUSY;
    }
    if (st != HAL_OK)
    {
        return ErrorCode::ERROR;
    }
    return ErrorCode::OK;
}

ErrorCode I2c::write_reg(uint8_t reg, const uint8_t* data, uint16_t len)
{
    if (data == nullptr || len == 0U)
    {
        return ErrorCode::INVALID_ARG;
    }

    uint8_t* p = const_cast<uint8_t*>(data);
    const uint16_t dev_addr = static_cast<uint16_t>(m_addr << 1);
    const HAL_StatusTypeDef st = HAL_I2C_Mem_Write(m_hi2c, dev_addr, reg,
                                                   I2C_MEMADD_SIZE_8BIT, p, len, 100);
    if (st == HAL_TIMEOUT)
    {
        return ErrorCode::TIMEOUT;
    }
    if (st == HAL_BUSY)
    {
        return ErrorCode::BUS_BUSY;
    }
    if (st != HAL_OK)
    {
        return ErrorCode::ERROR;
    }
    return ErrorCode::OK;
}

ErrorCode I2c::control(int32_t cmd, void* arg)
{
    switch (cmd)
    {
        case DEV_CTRL_SET_MODE:
            m_mode = *static_cast<TransMode*>(arg);
            return ErrorCode::OK;
        case DEV_CTRL_SET_ADDR:
            m_addr = *static_cast<uint8_t*>(arg);
            return ErrorCode::OK;
        case DEV_CTRL_RESET:
            if (HAL_I2C_DeInit(m_hi2c) != HAL_OK)
            {
                return ErrorCode::INIT_FAIL;
            }
            if (HAL_I2C_Init(m_hi2c) != HAL_OK)
            {
                return ErrorCode::INIT_FAIL;
            }
            return ErrorCode::OK;
        default:
            return ErrorCode::INVALID_ARG;
    }
}

ErrorCode I2c::set_callback(DevTransCallback cb, void* user_arg)
{
    m_cb = cb;
    m_user_arg = user_arg;
    return ErrorCode::OK;
}
}