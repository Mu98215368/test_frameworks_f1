#pragma once
#include "bsp/device/device.hpp"
#include "stm32f1xx_hal.h"

namespace bsp
{
// CAN 发送参数
struct CanTxParam
{
    uint32_t id = 0x123;   // 标准帧ID，默认 0x123
};

class Can : public Device
{
public:
    explicit Can(CAN_HandleTypeDef* hcan);

    ErrorCode init(uint8_t addr, TransMode mode) override;
    ErrorCode write(const uint8_t* buf, uint16_t len) override;
    ErrorCode read(uint8_t* buf, uint16_t len, uint16_t* read_len) override;
    ErrorCode control(int32_t cmd, void* arg) override;
    ErrorCode set_callback(DevTransCallback cb, void* user_arg) override;

    static void rx_wrapper(CAN_HandleTypeDef* hcan);

private:
    CAN_HandleTypeDef* m_hcan;
    CanTxParam m_tx_param;
    TransMode m_mode = TransMode::BLOCK;
    DevTransCallback m_cb = nullptr;
    void* m_user_arg = nullptr;
    static Can* g_instance;
};
}