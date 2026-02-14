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

void systick_init(void);
systick_t systick_get_ms(void);
systick_t systick_get_s(void);
uint8_t systick_is_timeout(systick_t start, uint32_t timeout_ms);

#endif
