/*
 * 应用层入口：硬件实例、业务模块实例、初始化与主循环
 */
#pragma once
#include "bsp/gpio/gpio.hpp"
#include "bsp/pwm/pwm.hpp"
#include "bsp/i2c/i2c.hpp"
#include "bsp/log/log.hpp"
#include "module/laser/laser.hpp"
#include "module/motor/motor.hpp"
#include "module/servo/servo.hpp"
#include "module/mpu6050/mpu6050.hpp"
#include "module/sensor/sensor.hpp"

namespace app
{
// ========== BSP 硬件实例 ==========
extern bsp::Gpio gpio_laser;
extern bsp::Pwm  pwm_servo;
extern bsp::Pwm  pwm_motor;
extern bsp::Gpio gpio_motor_in1;
extern bsp::Gpio gpio_motor_in2;
extern bsp::I2c  i2c1;

// ========== Module 业务实例 ==========
extern module::Laser   laser;
extern module::Servo   servo;
extern module::Motor   motor;
extern module::Mpu6050 mpu6050;

// 传感器列表：新增传感器只加一行，调度代码不用改
constexpr uint8_t SENSOR_CNT = 1;
extern module::Sensor* sensor_list[SENSOR_CNT];

// ========== 对外接口 ==========
void app_init(void);
void app_loop(void);
}