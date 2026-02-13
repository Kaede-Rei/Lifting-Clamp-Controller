/**
 * @file  systick.c
 * @brief SysTick 1 ms 心跳实现
 *        72 MHz / 72000 = 1 kHz
 */
#include "systick.h"

// ! ========================= 变 量 声 明 ========================= ! //

static volatile systick_t _ms = 0;

// ! ========================= 私 有 函 数 声 明 ========================= ! //



// ! ========================= 接 口 函 数 实 现 ========================= ! //

/**
 * @brief   初始化 SysTick
 * @param   None
 * @retval  None
 */
void systick_init(void) {
    SysTick_Config(72000);
    NVIC_SetPriority(SysTick_IRQn, 15);
    _ms = 0;
}

/**
 * @brief   获取毫秒级系统时间
 * @param   None
 * @retval  systick_t 毫秒数
 */
systick_t systick_get_ms(void) {
    return _ms;
}

/**
 * @brief   获取秒级系统时间
 * @param   None
 * @retval  systick_t 秒数
 */
systick_t systick_get_s(void) {
    return _ms / 1000;
}

/**
 * @brief   检查是否超时 (毫秒)
 * @param   start 起始时间
 * @param   timeout_ms 超时时间
 * @retval  uint8_t 1:超时, 0:未超时
 */
uint8_t systick_is_timeout(systick_t start, uint32_t timeout_ms) {
    systick_t now = _ms;
    if(now >= start)
        return (now - start) >= timeout_ms ? 1 : 0;
    else
        return ((0xFFFFFFFFU - start) + now + 1) >= timeout_ms ? 1 : 0;
}

/**
 * @brief   SysTick 中断服务
 * @param   None
 * @retval  None
 */
void SysTick_Handler(void) {
    _ms++;
}

// ! ========================= 私 有 函 数 实 现 ========================= ! //


