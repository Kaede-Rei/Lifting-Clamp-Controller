/**
 * @file  timer.h
 * @brief TIM3 周期定时器 (OOP 封装)
 *        10 ms 周期中断，驱动主循环节拍
 */
#ifndef _timer_h_
#define _timer_h_

#include "stm32f10x.h"
#include <stdint.h>

// ! ========================= 接 口 变 量 / Typedef 声 明 ========================= ! //

typedef void (*TimerCb)(void);

typedef struct Timer Timer;
struct Timer {
    /* public methods */
    void (*init)(Timer* self);
    void (*set_callback)(Timer* self, TimerCb cb);
    /* public data (ISR 置位, 主循环清零) */
    volatile uint8_t flag_;
    /* private data */
    TimerCb callback_;
};

// ! ========================= 接 口 函 数 声 明 ========================= ! //

/**
 * @brief   创建 Timer 对象
 * @param   None
 * @retval  Timer 对象
 */
Timer timer_create(void);

#endif
