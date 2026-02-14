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
// public:
    /**
     * @brief   初始化定时器
     * @param   self 定时器对象
     * @retval  None
     */
    void (*init)(Timer* self);
    /**
     * @brief   设置回调函数
     * @param   self 定时器对象
     * @param   cb 回调函数
     * @retval  None
     */
    void (*set_callback)(Timer* self, TimerCb cb);

    volatile uint8_t flag_;

// private:
    TimerCb _callback_;
};

// ! ========================= 接 口 函 数 声 明 ========================= ! //

Timer timer_create(void);

#endif
