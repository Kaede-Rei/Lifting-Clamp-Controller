/**
 * @file  s_delay.h
 * @brief 延时服务 (阻塞 & 非阻塞)
 */
#ifndef _s_delay_h_
#define _s_delay_h_

#include <stdint.h>
#include "systick.h"

// ! ========================= 接 口 函 数 声 明 ========================= ! //

/**
 * @brief   微秒级阻塞延时
 * @param   us 延时微秒数
 * @retval  None
 */
void    s_delay_us(uint32_t us);

/**
 * @brief   毫秒级阻塞延时
 * @param   ms 延时毫秒数
 * @retval  None
 */
void    s_delay_ms(uint32_t ms);

/**
 * @brief   秒级阻塞延时
 * @param   s 延时秒数
 * @retval  None
 */
void    s_delay_s(uint32_t s);

/**
 * @brief   非阻塞微秒延时
 * @param   start 指向起始时间的指针
 * @param   interval_us 延时间隔(us)
 * @retval  uint8_t 1:时间到, 0:未到
 */
uint8_t s_nb_delay_us(uint32_t* start, uint32_t interval_us);

/**
 * @brief   非阻塞毫秒延时
 * @param   start 指向起始时间的指针
 * @param   interval_ms 延时间隔(ms)
 * @retval  uint8_t 1:时间到, 0:未到
 */
uint8_t s_nb_delay_ms(systick_t* start, uint32_t interval_ms);

#endif
