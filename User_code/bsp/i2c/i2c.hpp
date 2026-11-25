#pragma once
#include "bsp/device/device.hpp"
#include "stm32f1xx_hal.h"

namespace bsp
{
class I2c : public Device
{
public:
    explicit I2c(I2C_HandleTypeDef* hi2c);

    ErrorCode init(uint8_t addr, TransMode mode) override;
    ErrorCode read(uint8_t* buf, uint16_t len, uint16_t* read_len) override;
    ErrorCode write(const uint8_t* buf, uint16_t len) override;
    ErrorCode control(int32_t cmd, void* arg) override;
    ErrorCode set_callback(DevTransCallback cb, void* user_arg) override;
    ErrorCode read_reg(uint8_t reg, uint8_t* buf, uint16_t len) override;
    ErrorCode write_reg(uint8_t reg, const uint8_t* data, uint16_t len) override;

private:
    I2C_HandleTypeDef* m_hi2c;
    uint8_t m_addr = 0;
    TransMode m_mode = TransMode::BLOCK;
    DevTransCallback m_cb = nullptr;
    void* m_user_arg = nullptr;
};
}