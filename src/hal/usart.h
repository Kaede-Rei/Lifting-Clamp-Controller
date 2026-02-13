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
    /* public methods */
    void    (*init)(Usart* self);
    void    (*send_byte)(Usart* self, uint8_t byte);
    void    (*send_string)(Usart* self, const char* str);
    uint8_t(*read_byte)(Usart* self, uint8_t* out);
    /* private data */
    USART_TypeDef* periph_;
    uint32_t       baudrate_;
    uint8_t        rx_buf_[USART_RX_BUF_SIZE];
    volatile uint16_t rx_head_;
    volatile uint16_t rx_tail_;
};

// ! ========================= 接 口 函 数 声 明 ========================= ! //

/**
 * @brief   创建 USART1 对象
 * @param   baudrate 波特率
 * @retval  Usart 对象
 */
Usart usart1_create(uint32_t baudrate);

/**
 * @brief   创建 USART2 对象
 * @param   baudrate 波特率
 * @retval  Usart 对象
 */
Usart usart2_create(uint32_t baudrate);

#endif
