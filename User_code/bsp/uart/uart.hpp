#pragma once
#include "bsp/device/device.hpp"
#include "stm32f1xx_hal.h"

namespace bsp
{
class Uart : public Device
{
public:
    explicit Uart(UART_HandleTypeDef* huart);

    ErrorCode init(uint8_t addr, TransMode mode) override;
    ErrorCode write(const uint8_t* buf, uint16_t len) override;
    ErrorCode read(uint8_t* buf, uint16_t len, uint16_t* read_len) override;
    ErrorCode control(int32_t cmd, void* arg) override;
    ErrorCode set_callback(DevTransCallback cb, void* user_arg) override;

    // HAL中断回调包装函数（public static）
    static void rx_wrapper(UART_HandleTypeDef* huart);
    static void tx_wrapper(UART_HandleTypeDef* huart);

private:
    UART_HandleTypeDef* m_huart;
    TransMode m_mode = TransMode::BLOCK;
    DevTransCallback m_cb = nullptr;
    void* m_user_arg = nullptr;
    static Uart* g_instance;
};
}