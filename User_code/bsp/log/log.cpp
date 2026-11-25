#include "bsp/log/log.hpp"
#include "bsp/uart/uart.hpp"
#include <cstdarg>
#include <cstdio>
#include "usart.h"

namespace bsp
{
// 日志串口实例：全局唯一定义在这里，避免链接时 undefined reference
Uart g_uart_log(&huart1);

static char log_buf[128];

void log_info(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int len = vsnprintf(log_buf, sizeof(log_buf), fmt, ap);
    va_end(ap);

    // 截断保护：返回值 >= 缓冲区大小代表内容被截断
    if (len > static_cast<int>(sizeof(log_buf)) - 1)
    {
        len = static_cast<int>(sizeof(log_buf)) - 1;
    }

    if (len > 0)
    {
        g_uart_log.write(reinterpret_cast<const uint8_t*>(log_buf), static_cast<uint16_t>(len));
    }
}

void log_error(ErrorCode ec, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int len = snprintf(log_buf, sizeof(log_buf), "[ERR:%d] ", static_cast<int>(ec));

    if (len > 0 && len < static_cast<int>(sizeof(log_buf)))
    {
        int add_len = vsnprintf(log_buf + len, sizeof(log_buf) - len, fmt, ap);
        if (add_len > 0)
        {
            if (len + add_len >= static_cast<int>(sizeof(log_buf)))
            {
                add_len = static_cast<int>(sizeof(log_buf)) - 1 - len;
            }
            len += add_len;
        }
    }
    va_end(ap);

    if (len > 0)
    {
        g_uart_log.write(reinterpret_cast<const uint8_t*>(log_buf), static_cast<uint16_t>(len));
    }
}
}