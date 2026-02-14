/**
 * @file  s_delay.h
 * @brief 延时服务 (阻塞 & 非阻塞)
 */
#ifndef _s_delay_h_
#define _s_delay_h_

#include <stdint.h>
#include "systick.h"

// ! ========================= 接 口 函 数 声 明 ========================= ! //

void s_delay_us(uint32_t us);
void s_delay_ms(uint32_t ms);
void s_delay_s(uint32_t s);
uint8_t s_nb_delay_us(uint32_t* start, uint32_t interval_us);
uint8_t s_nb_delay_ms(systick_t* start, uint32_t interval_ms);

#endif
