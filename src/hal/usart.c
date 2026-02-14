/**
 * @file  usart.c
 * @brief USART 外设驱动实现
 *        USART1: PA9-TX  PA10-RX  (无线串口, 中断接收)
 *        USART2: PA2-TX  PA3-RX   (夹爪舵机, 仅发送)
 */
#include "usart.h"
#include <stdio.h>

// ! ========================= 变 量 声 明 ========================= ! //

static Usart* _usart1_inst = 0;

// ! ========================= 私 有 函 数 声 明 ========================= ! //

static void _send_byte(Usart* self, uint8_t byte);
static void _send_string(Usart* self, const char* str);
static uint8_t _read_byte(Usart* self, uint8_t* out);
static void _usart1_init(Usart* self);
static void _usart2_init(Usart* self);

// ! ========================= 接 口 函 数 实 现 ========================= ! //

/**
 * @brief   创建 USART1 对象
 * @param   baudrate 波特率
 * @retval  Usart 对象
 */
Usart usart1_create(uint32_t baudrate) {
    Usart obj;
    obj._periph_ = USART1;
    obj._baudrate_ = baudrate;
    obj._rx_head_ = 0;
    obj._rx_tail_ = 0;
    obj.init = _usart1_init;
    obj.send_byte = _send_byte;
    obj.send_string = _send_string;
    obj.read_byte = _read_byte;
    return obj;
}

/**
 * @brief   创建 USART2 对象
 * @param   baudrate 波特率
 * @retval  Usart 对象
 */
Usart usart2_create(uint32_t baudrate) {
    Usart obj;
    obj._periph_ = USART2;
    obj._baudrate_ = baudrate;
    obj._rx_head_ = 0;
    obj._rx_tail_ = 0;
    obj.init = _usart2_init;
    obj.send_byte = _send_byte;
    obj.send_string = _send_string;
    obj.read_byte = _read_byte;
    return obj;
}

/**
 * @brief   USART1 中断服务
 * @param   None
 * @retval  None
 */
void USART1_IRQHandler(void) {
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        uint8_t data = (uint8_t)USART_ReceiveData(USART1);
        if(_usart1_inst) {
            uint16_t next = (_usart1_inst->_rx_head_ + 1) % USART_RX_BUF_SIZE;
            if(next != _usart1_inst->_rx_tail_) {
                _usart1_inst->_rx_buf_[_usart1_inst->_rx_head_] = data;
                _usart1_inst->_rx_head_ = next;
            }
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

// ! ========================= printf 重 定 向 ========================= ! //

#pragma import(__use_no_semihosting)

struct __FILE { int handle; };
FILE __stdout;

void _sys_exit(int x) { x = x; }

int fputc(int ch, FILE* f) {
    (void)f;
    while((USART1->SR & 0x40) == 0);
    USART1->DR = (uint8_t)ch;
    return ch;
}

// ! ========================= 私 有 函 数 实 现 ========================= ! //

/**
 * @brief   发送单字节
 * @param   self 串口对象
 * @param   byte 字节数据
 * @retval  None
 */
static void _send_byte(Usart* self, uint8_t byte) {
    while(USART_GetFlagStatus(self->_periph_, USART_FLAG_TC) == RESET);
    USART_SendData(self->_periph_, byte);
}

/**
 * @brief   发送字符串
 * @param   self 串口对象
 * @param   str 字符串
 * @retval  None
 */
static void _send_string(Usart* self, const char* str) {
    while(*str) {
        _send_byte(self, (uint8_t)*str++);
    }
}

/**
 * @brief   读取单字节
 * @param   self 串口对象
 * @param   out 输出缓冲区
 * @retval  uint8_t 1:成功, 0:缓冲区空
 */
static uint8_t _read_byte(Usart* self, uint8_t* out) {
    if(self->_rx_head_ == self->_rx_tail_) return 0;
    *out = self->_rx_buf_[self->_rx_tail_];
    self->_rx_tail_ = (self->_rx_tail_ + 1) % USART_RX_BUF_SIZE;
    return 1;
}

/**
 * @brief   初始化 USART1
 * @param   self 串口对象
 * @retval  None
 */
static void _usart1_init(Usart* self) {
    _usart1_inst = self;
    self->_rx_head_ = 0;
    self->_rx_tail_ = 0;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    USART_DeInit(USART1);

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = GPIO_Pin_9;           /* PA9 TX */
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_10;          /* PA10 RX */
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio);

    USART_InitTypeDef ui;
    ui.USART_BaudRate = self->_baudrate_;
    ui.USART_WordLength = USART_WordLength_8b;
    ui.USART_StopBits = USART_StopBits_1;
    ui.USART_Parity = USART_Parity_No;
    ui.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    ui.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &ui);

    NVIC_InitTypeDef ni;
    ni.NVIC_IRQChannel = USART1_IRQn;
    ni.NVIC_IRQChannelPreemptionPriority = 3;
    ni.NVIC_IRQChannelSubPriority = 3;
    ni.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&ni);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

/**
 * @brief   初始化 USART2
 * @param   self 串口对象
 * @retval  None
 */
static void _usart2_init(Usart* self) {
    self->_rx_head_ = 0;
    self->_rx_tail_ = 0;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = GPIO_Pin_2;           /* PA2 TX */
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_3;           /* PA3 RX */
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio);

    USART_InitTypeDef ui;
    ui.USART_BaudRate = self->_baudrate_;
    ui.USART_WordLength = USART_WordLength_8b;
    ui.USART_StopBits = USART_StopBits_1;
    ui.USART_Parity = USART_Parity_No;
    ui.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    ui.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART2, &ui);

    USART_Cmd(USART2, ENABLE);
}
