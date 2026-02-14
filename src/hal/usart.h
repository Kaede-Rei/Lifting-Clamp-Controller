/**
 * @file  usart.h
 * @brief USART 外设驱动 (OOP 封装)
 *        USART1: PA9-TX  PA10-RX  (无线串口)
 *        USART2: PA2-TX  PA3-RX   (夹爪舵机)
 */
#ifndef _usart_h_
#define _usart_h_

#include "stm32f10x.h"
#include <stdint.h>

// ! ========================= 接 口 变 量 / Typedef 声 明 ========================= ! //

/**
 * @brief USART接收缓冲区大小定义
 */
#define USART_RX_BUF_SIZE  128

typedef struct Usart Usart;
struct Usart {
// public:
    /**
     * @brief   初始化 USART
     * @param   self 串口对象
     * @retval  None
     */
    void(*init)(Usart* self);
    /**
     * @brief   发送单字节
     * @param   self 串口对象
     * @param   byte 字节数据
     * @retval  None
     */
    void(*send_byte)(Usart* self, uint8_t byte);
    /**
     * @brief   发送字符串
     * @param   self 串口对象
     * @param   str 字符串
     * @retval  None
     */
    void(*send_string)(Usart* self, const char* str);
    /**
     * @brief   读取单字节
     * @param   self 串口对象
     * @param   out 输出缓冲区
     * @retval  uint8_t 1:成功, 0:缓冲区空
     */
    uint8_t(*read_byte)(Usart* self, uint8_t* out);

// private:
    USART_TypeDef* _periph_;
    uint32_t _baudrate_;
    uint8_t  _rx_buf_[USART_RX_BUF_SIZE];
    volatile uint16_t _rx_head_;
    volatile uint16_t _rx_tail_;
};

// ! ========================= 接 口 函 数 声 明 ========================= ! //

Usart usart1_create(uint32_t baudrate);
Usart usart2_create(uint32_t baudrate);

#endif
