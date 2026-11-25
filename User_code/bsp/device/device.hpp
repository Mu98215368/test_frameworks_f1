#pragma once
#include "bsp/error/error.hpp"
#include <cstdint>

namespace bsp
{
// 传输模式枚举，放到bsp命名空间
enum class TransMode
{
    BLOCK,  // 阻塞
    IT      // 中断/异步
};

class Device
{
public:
    virtual ~Device() = default;

    /**
     * @brief 初始化设备
     * @param addr 从机地址
     * @param mode 传输模式
     */
    virtual ErrorCode init(uint8_t addr, TransMode mode) = 0;

    /**
     * @brief 读数据
     * @param buf 接收缓冲区
     * @param len 期望读取长度
     * @param read_len 实际读到长度（出参）
     * @note BLOCK: 等待到数据返回；IT: 发起接收立即返回，完成走回调
     */
    virtual ErrorCode read(uint8_t* buf, uint16_t len, uint16_t* read_len) = 0;

    /**
     * @brief 写数据
     * @note BLOCK: 等待完成才返回；IT: 发起传输立即返回，完成走回调
     */
    virtual ErrorCode write(const uint8_t* buf, uint16_t len) = 0;

    /**
     * @brief 设备控制
     * @param cmd 控制命令，见 DeviceCtrlCmd
     */
    virtual ErrorCode control(int32_t cmd, void* arg) = 0;

    /**
     * @brief 注册传输完成回调（IT模式使用）
     */
    using DevTransCallback = ErrorCode (*)(void* user_arg);
    virtual ErrorCode set_callback(DevTransCallback cb, void* user_arg) = 0;

    /**
     * @brief 按寄存器地址读（I2C/SPI 这类寄存器型设备重写）
     * @note 默认返回 INVALID_ARG，表示该设备不支持寄存器访问
     */
    virtual ErrorCode read_reg(uint8_t reg, uint8_t* buf, uint16_t len)
    {
        (void)reg;
        (void)buf;
        (void)len;
        return ErrorCode::INVALID_ARG;
    }

    /**
     * @brief 按寄存器地址写
     */
    virtual ErrorCode write_reg(uint8_t reg, const uint8_t* data, uint16_t len)
    {
        (void)reg;
        (void)data;
        (void)len;
        return ErrorCode::INVALID_ARG;
    }
};

// 设备控制命令
enum DeviceCtrlCmd : int32_t
{
    DEV_CTRL_RESET      = 0x0001,
    DEV_CTRL_SET_BAUD   = 0x0002,
    DEV_CTRL_SET_ADDR   = 0x0003,
    DEV_CTRL_CAN_FILTER = 0x0004,
    DEV_CTRL_START      = 0x0005,
    DEV_CTRL_STOP       = 0x0006,
    DEV_CTRL_SET_MODE   = 0x0007,
    DEV_CTRL_SET_ID     = 0x0008
};

}