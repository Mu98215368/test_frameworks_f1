#pragma once
#include "bsp/error/error.hpp"

namespace bsp
{
void log_info(const char* fmt, ...);
void log_error(ErrorCode ec, const char* fmt, ...);
}

/*#include "bsp/uart/uart.hpp"
#include "bsp/log/log.hpp"
extern UART_HandleTypeDef huart1;

// 全局日志串口实例
bsp::Uart g_uart_log(&huart1);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_USART1_UART_Init();

    // 日志固定使用阻塞模式（默认就是BLOCK，这一行可以不写，写上更清晰）
    bsp::TransMode log_mode = bsp::TransMode::BLOCK;
    g_uart_log.control(bsp::DEV_CTRL_SET_MODE, &log_mode);

    log_info("System Start\r\n");
    log_error(ErrorCode::TIMEOUT, "Test error log\r\n");

    while(1)
    {

    }
}
*/