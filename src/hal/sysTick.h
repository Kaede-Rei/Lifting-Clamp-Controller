/**
 * @file  systick.h
 * @brief SysTick 1 ms 心跳计时
 */
#ifndef _sysTick_h_
#define _sysTick_h_

#include "stm32f10x.h"
#include <stdint.h>

// ! ========================= 接 口 变 量 / Typedef 声 明 ========================= ! //

typedef uint32_t systick_t;

// ! ========================= 接 口 函 数 声 明 ========================= ! //

/**
 * @brief   初始化 SysTick
 * @param   None
 * @retval  None
 */
void      systick_init(void);

/**
 * @brief   获取毫秒级系统时间
 * @param   None
 * @retval  systick_t 毫秒数
 */
systick_t systick_get_ms(void);

/**
 * @brief   获取秒级系统时间
 * @param   None
 * @retval  systick_t 秒数
 */
systick_t systick_get_s(void);

/**
 * @brief   检查是否超时 (毫秒)
 * @param   start 起始时间
 * @param   timeout_ms 超时时间
 * @retval  uint8_t 1:超时, 0:未超时
 */
uint8_t   systick_is_timeout(systick_t start, uint32_t timeout_ms);

#endif
