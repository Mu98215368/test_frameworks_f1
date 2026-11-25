#include "module/mpu6050/mpu6050.hpp"
#include "bsp/log/log.hpp"

namespace module
{
namespace
{
constexpr uint8_t  REG_PWR_MGMT_1 = 0x6B;
constexpr uint8_t  REG_WHO_AM_I   = 0x75;
constexpr uint8_t  REG_ACCEL_XOUT = 0x3B;
constexpr uint8_t  WHO_AM_I_VAL   = 0x68;
constexpr uint16_t RAW_LEN        = 14;   // 加速度6 + 温度2 + 陀螺6
}

Mpu6050::Mpu6050(bsp::Device* dev)
    : m_dev(dev)
{}

bsp::ErrorCode Mpu6050::config(uint8_t slave_addr, bsp::TransMode mode)
{
    m_addr = slave_addr;
    m_mode = mode;
    return bsp::ErrorCode::OK;
}

bsp::ErrorCode Mpu6050::init()
{
    if (m_dev == nullptr)
    {
        return bsp::ErrorCode::INVALID_ARG;
    }

    auto ret = m_dev->init(m_addr, m_mode);
    if (ret != bsp::ErrorCode::OK)
    {
        bsp::log_error(ret, "mpu6050 dev init fail\r\n");
        return ret;
    }

    // 解除休眠：PWR_MGMT_1 = 0x00
    uint8_t wake = 0x00;
    ret = m_dev->write_reg(REG_PWR_MGMT_1, &wake, 1);
    if (ret != bsp::ErrorCode::OK)
    {
        bsp::log_error(ret, "mpu6050 wake fail\r\n");
        return ret;
    }

    // 读 WHO_AM_I，确认芯片在线
    uint8_t who = 0x00;
    ret = m_dev->read_reg(REG_WHO_AM_I, &who, 1);
    if (ret != bsp::ErrorCode::OK)
    {
        bsp::log_error(ret, "mpu6050 read who_am_i fail\r\n");
        return ret;
    }
    if (who != WHO_AM_I_VAL)
    {
        bsp::log_error(bsp::ErrorCode::CHECK_FAIL, "mpu6050 who_am_i = 0x%02X\r\n", who);
        return bsp::ErrorCode::CHECK_FAIL;
    }

    return bsp::ErrorCode::OK;
}

bsp::ErrorCode Mpu6050::read()
{
    if (m_dev == nullptr)
    {
        return bsp::ErrorCode::INVALID_ARG;
    }

    uint8_t raw[RAW_LEN] = {0};
    auto ret = m_dev->read_reg(REG_ACCEL_XOUT, raw, RAW_LEN);
    if (ret != bsp::ErrorCode::OK)
    {
        bsp::log_error(ret, "mpu6050 read fail\r\n");
        return ret;
    }

    m_buf.ax = static_cast<int16_t>((raw[0] << 8) | raw[1]);
    m_buf.ay = static_cast<int16_t>((raw[2] << 8) | raw[3]);
    m_buf.az = static_cast<int16_t>((raw[4] << 8) | raw[5]);
    // raw[6], raw[7] 是温度，跳过
    m_buf.gx = static_cast<int16_t>((raw[8]  << 8) | raw[9]);
    m_buf.gy = static_cast<int16_t>((raw[10] << 8) | raw[11]);
    m_buf.gz = static_cast<int16_t>((raw[12] << 8) | raw[13]);

    return bsp::ErrorCode::OK;
}

void Mpu6050::get_data(Mpu6050Data* dat)
{
    if (dat != nullptr)
    {
        *dat = m_buf;
    }
}
}