/**
 * @file  dwt.c
 * @brief DWT 微秒级计时器实现
 */
#include "dwt.h"

// ! ========================= 变 量 声 明 ========================= ! //



// ! ========================= 私 有 函 数 声 明 ========================= ! //



// ! ========================= 接 口 函 数 实 现 ========================= ! //

/**
 * @brief   初始化 DWT
 * @param   None
 * @retval  None
 */
void dwt_init(void) {
    if(!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk))
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0;
}

/**
 * @brief   获取系统运行微秒数
 * @param   None
 * @retval  uint32_t 微秒数
 */
uint32_t dwt_get_us(void) {
    return DWT->CYCCNT / CPU_FREQ_MHZ;
}

/**
 * @brief   检查是否超时 (微秒)
 * @param   start 起始时间
 * @param   timeout_us 超时时间
 * @retval  uint8_t 1:超时, 0:未超时
 */
uint8_t dwt_is_timeout(uint32_t start, uint32_t timeout_us) {
    uint32_t now = dwt_get_us();
    if(now >= start)
        return (now - start) >= timeout_us ? 1 : 0;
    else
        return ((0xFFFFFFFFU - start) + now + 1) >= timeout_us ? 1 : 0;
}

// ! ========================= 私 有 函 数 实 现 ========================= ! //


