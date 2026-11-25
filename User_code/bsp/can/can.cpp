#include "bsp/can/can.hpp"

namespace bsp
{
Can* Can::g_instance = nullptr;

Can::Can(CAN_HandleTypeDef* hcan)
    : m_hcan(hcan)
{
    g_instance = this;
}

ErrorCode Can::init(uint8_t addr, TransMode mode)
{
    (void)addr;
    m_mode = mode;
    if (m_hcan == nullptr)
    {
        return ErrorCode::INVALID_ARG;
    }
    if (HAL_CAN_Start(m_hcan) != HAL_OK)
    {
        return ErrorCode::INIT_FAIL;
    }
    return ErrorCode::OK;
}

ErrorCode Can::set_callback(DevTransCallback cb, void* user_arg)
{
    m_cb = cb;
    m_user_arg = user_arg;
    return ErrorCode::OK;
}

ErrorCode Can::write(const uint8_t* buf, uint16_t len)
{
    if (buf == nullptr || len == 0U || len > 8U)
    {
        return ErrorCode::INVALID_ARG;
    }

    CAN_TxHeaderTypeDef tx_header;
    uint32_t tx_mailbox;
    tx_header.StdId = m_tx_param.id;
    tx_header.ExtId = 0;
    tx_header.IDE = CAN_ID_STD;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.DLC = static_cast<uint8_t>(len);

    if (HAL_CAN_AddTxMessage(m_hcan, &tx_header, buf, &tx_mailbox) != HAL_OK)
    {
        return ErrorCode::BUS_BUSY;
    }
    return ErrorCode::OK;
}

ErrorCode Can::read(uint8_t* buf, uint16_t len, uint16_t* read_len)
{
    if (buf == nullptr || read_len == nullptr || len == 0U)
    {
        return ErrorCode::INVALID_ARG;
    }
    *read_len = 0;

    if (m_mode == TransMode::BLOCK)
    {
        CAN_RxHeaderTypeDef rx_header;
        if (HAL_CAN_GetRxMessage(m_hcan, CAN_RX_FIFO0, &rx_header, buf) != HAL_OK)
        {
            return ErrorCode::NO_DATA;
        }
        *read_len = rx_header.DLC;
        return ErrorCode::OK;
    }
    else if (m_mode == TransMode::IT)
    {
        // 只开中断，数据在回调里用 BLOCK 方式取
        if (HAL_CAN_ActivateNotification(m_hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
        {
            return ErrorCode::INIT_FAIL;
        }
        return ErrorCode::OK;
    }
    return ErrorCode::INVALID_ARG;
}

ErrorCode Can::control(int32_t cmd, void* arg)
{
    switch (cmd)
    {
        case DEV_CTRL_SET_MODE:
            m_mode = *static_cast<TransMode*>(arg);
            return ErrorCode::OK;
        case DEV_CTRL_SET_ID:
            m_tx_param.id = *static_cast<uint32_t*>(arg);
            return ErrorCode::OK;
        case DEV_CTRL_START:
            if (HAL_CAN_Start(m_hcan) != HAL_OK)
            {
                return ErrorCode::INIT_FAIL;
            }
            return ErrorCode::OK;
        case DEV_CTRL_STOP:
            if (HAL_CAN_Stop(m_hcan) != HAL_OK)
            {
                return ErrorCode::INIT_FAIL;
            }
            return ErrorCode::OK;
        case DEV_CTRL_CAN_FILTER:
            // 过滤器在 CubeMX / MX_CAN_Init 里配置，这里留给运行时改
            return ErrorCode::OK;
        default:
            return ErrorCode::INVALID_ARG;
    }
}

void Can::rx_wrapper(CAN_HandleTypeDef* hcan)
{
    (void)hcan;
    if (g_instance != nullptr && g_instance->m_cb != nullptr)
    {
        g_instance->m_cb(g_instance->m_user_arg);
    }
}
}