/*
 * 应用层实现：BSP 硬件实例 -> Module 业务实例 -> 初始化与调度
 */
#include "app/app.hpp"
#include "app/app_bridge.h"

// CubeMX 生成的外设句柄（这些头文件自带 extern "C" 保护）
#include "gpio.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"

namespace app
{
// ---------------- BSP 硬件构造 ----------------
bsp::Gpio gpio_laser(GPIOA, GPIO_PIN_0);

// TIM1：50Hz，ARR=999，舵机 0.5~2.5ms -> 25~125
bsp::Pwm  pwm_servo(&htim1, TIM_CHANNEL_1);

// TIM3：1kHz，ARR=999，电机调速 0~999
bsp::Pwm  pwm_motor(&htim3, TIM_CHANNEL_1);

// H桥方向脚：按实际接线修改引脚，并在 CubeMX 里配成 GPIO_Output
bsp::Gpio gpio_motor_in1(GPIOA, GPIO_PIN_1);
bsp::Gpio gpio_motor_in2(GPIOA, GPIO_PIN_4);

bsp::I2c  i2c1(&hi2c1);

// ---------------- Module 构造（注入 BSP 指针） ----------------
module::Laser    laser(&gpio_laser);
module::Servo    servo(&pwm_servo, 999);
module::Motor    motor(&pwm_motor, &gpio_motor_in1, &gpio_motor_in2);
module::Mpu6050  mpu6050(&i2c1);

module::Sensor* sensor_list[SENSOR_CNT] = { &mpu6050 };

void app_init(void)
{
    bsp::ErrorCode ret;

    ret = laser.init();
    if (ret != bsp::ErrorCode::OK)
    {
        bsp::log_error(ret, "laser init fail\r\n");
    }

    ret = servo.init();
    if (ret != bsp::ErrorCode::OK)
    {
        bsp::log_error(ret, "servo init fail\r\n");
    }

    ret = motor.init();
    if (ret != bsp::ErrorCode::OK)
    {
        bsp::log_error(ret, "motor init fail\r\n");
    }

    mpu6050.config(0x68, bsp::TransMode::BLOCK);
    ret = mpu6050.init();
    if (ret != bsp::ErrorCode::OK)
    {
        bsp::log_error(ret, "mpu6050 init fail\r\n");
    }

    bsp::log_info("app init complete\r\n");
}

void app_loop(void)
{
    static uint32_t tick = 0;

    // 10ms 调度一次传感器
    if (HAL_GetTick() - tick >= 10)
    {
        tick = HAL_GetTick();

        for (uint8_t i = 0; i < SENSOR_CNT; i++)
        {
            if (sensor_list[i] == nullptr)
            {
                continue;
            }
            auto ret = sensor_list[i]->read();
            if (ret != bsp::ErrorCode::OK)
            {
                bsp::log_error(ret, "sensor[%u] read fail\r\n", i);
            }
        }
    }

    // 业务动作放这里，例如：
    // laser.on();
    // servo.set_angle(90.0f);
    // motor.forward(500);
}
}

/* 给 C 文件（freertos.c / main.c）用的桥接实现 */
extern "C" void app_init_c(void)
{
    app::app_init();
}

extern "C" void app_loop_c(void)
{
    app::app_loop();
}