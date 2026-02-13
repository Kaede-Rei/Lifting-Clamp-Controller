/**
 * @file  s_delay.c
 * @brief 延时服务实现
 */
#include "s_delay.h"
#include "dwt.h"

// ! ========================= 变 量 声 明 ========================= ! //



// ! ========================= 私 有 函 数 声 明 ========================= ! //



// ! ========================= 接 口 函 数 实 现 ========================= ! //

/**
 * @brief   微秒级阻塞延时
 * @param   us 延时微秒数
 * @retval  None
 */
void s_delay_us(uint32_t us) {
    uint32_t start = dwt_get_us();
    while(!dwt_is_timeout(start, us));
}

/**
 * @brief   毫秒级阻塞延时
 * @param   ms 延时毫秒数
 * @retval  None
 */
void s_delay_ms(uint32_t ms) {
    systick_t start = systick_get_ms();
    while(!systick_is_timeout(start, ms));
}

/**
 * @brief   秒级阻塞延时
 * @param   s 延时秒数
 * @retval  None
 */
void s_delay_s(uint32_t s) {
    systick_t start = systick_get_ms();
    while(!systick_is_timeout(start, s * 1000));
}

/**
 * @brief   非阻塞微秒延时
 * @param   start 指向起始时间的指针
 * @param   interval_us 延时间隔(us)
 * @retval  uint8_t 1:时间到, 0:未到
 */
uint8_t s_nb_delay_us(uint32_t* start, uint32_t interval_us) {
    if(*start == 0) {
        *start = dwt_get_us();
        return 0;
    }
    if(dwt_is_timeout(*start, interval_us)) {
        *start = 0;
        return 1;
    }
    return 0;
}

/**
 * @brief   非阻塞毫秒延时
 * @param   start 指向起始时间的指针
 * @param   interval_ms 延时间隔(ms)
 * @retval  uint8_t 1:时间到, 0:未到
 */
uint8_t s_nb_delay_ms(systick_t* start, uint32_t interval_ms) {
    if(*start == 0) {
        *start = systick_get_ms();
        return 0;
    }
    if(systick_is_timeout(*start, interval_ms)) {
        *start = 0;
        return 1;
    }
    return 0;
}

// ! ========================= 私 有 函 数 实 现 ========================= ! //


