#pragma once
#include "bsp/error/error.hpp"
#include "bsp/device/device.hpp"
#include "module/sensor/sensor.hpp"

namespace module
{
struct Mpu6050Data
{
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
};

class Mpu6050 : public Sensor
{
public:
    Mpu6050(bsp::Device* dev);

    bsp::ErrorCode init() override;
    bsp::ErrorCode read() override;

    bsp::ErrorCode config(uint8_t slave_addr, bsp::TransMode mode);
    void get_data(Mpu6050Data* dat);

private:
    bsp::Device* m_dev;
    uint8_t m_addr = 0x68;
    bsp::TransMode m_mode = bsp::TransMode::BLOCK;
    Mpu6050Data m_buf;
};
}
