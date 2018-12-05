#include "headfile.h"

//串口调试系统
void Debug_Init(void)
{
    uart_init(uart0, 115200);
}

//串口数据读取
