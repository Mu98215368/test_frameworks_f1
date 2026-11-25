#pragma once
#include "bsp/error/error.hpp"

namespace module
{
class Sensor
{
public:
    virtual ~Sensor() = default;
    // 初始化传感器
    virtual bsp::ErrorCode init() = 0;
    // 读取数据，由子类实现
    virtual bsp::ErrorCode read() = 0;
    // 获取最新读到的数据（不同传感器数据不一样，子类自己提供get接口）
};
}
