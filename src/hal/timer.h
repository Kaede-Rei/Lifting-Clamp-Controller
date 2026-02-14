/**
 * @file  timer.h
 * @brief 通用定时器 HAL — 配置表驱动
 *        支持 TIM1 ~ TIM4 周期中断
 */
#ifndef _timer_h_
#define _timer_h_

#include "stm32f10x.h"
#include <stdint.h>

// ! ========================= 接 口 变 量 / Typedef 声 明 ========================= ! //

typedef void (*tim_cb_t)(void);

/**
 * @brief 定时器 ID 枚举
 */
typedef enum {
    TIM_1 = 0,
    TIM_2,
    TIM_3,
    TIM_4,
    TIM_COUNT
} tim_id_e;

/**
 * @brief 定时器配置表
 */
typedef struct {
    tim_id_e  id;           // 定时器 ID
    uint16_t prescaler;     // 预分频系数
    uint16_t period;        // 自动重装值
    uint8_t nvic_preempt;   // 抢占优先级
    uint8_t nvic_sub;       // 子优先级
} tim_cfg_t;

/**
 * @brief 定时器运行时句柄
 */
typedef struct {
    const tim_cfg_t* cfg;   // 指向配置表
    volatile uint8_t flag;  // 中断标志
    tim_cb_t callback;      // 中断回调
} tim_t;

// ! ========================= 接 口 函 数 声 明 ========================= ! //

void tim_init(tim_t* handle, const tim_cfg_t* cfg);
void tim_set_callback(tim_t* handle, tim_cb_t cb);

#endif
