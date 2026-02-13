/**
 * @file  dwt.h
 * @brief DWT 微秒级计时器
 */
#ifndef _dwt_h_
#define _dwt_h_

#include "stm32f10x.h"
#include <stdint.h>

// ! ========================= 接 口 变 量 / Typedef 声 明 ========================= ! //

#define CPU_FREQ_MHZ  72

#ifndef DWT_BASE
#define DWT_BASE  (0xE0001000UL)
#define DWT       ((DWT_Type *)DWT_BASE)
typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t CYCCNT;
    volatile uint32_t CPICNT;
    volatile uint32_t EXCCNT;
    volatile uint32_t SLEEPCNT;
    volatile uint32_t LSUCNT;
    volatile uint32_t FOLDCNT;
    volatile uint32_t PCSR;
} DWT_Type;
#endif

#ifndef DWT_CTRL_CYCCNTENA_Msk
#define DWT_CTRL_CYCCNTENA_Msk  (1UL << 0)
#endif

#ifndef CoreDebug_DEMCR_TRCENA_Msk
#define CoreDebug_DEMCR_TRCENA_Msk  (1UL << 24)
#endif

// ! ========================= 接 口 函 数 声 明 ========================= ! //

/**
 * @brief   初始化 DWT
 * @param   None
 * @retval  None
 */
void     dwt_init(void);

/**
 * @brief   获取系统运行微秒数
 * @param   None
 * @retval  uint32_t 微秒数
 */
uint32_t dwt_get_us(void);

/**
 * @brief   检查是否超时 (微秒)
 * @param   start 起始时间
 * @param   timeout_us 超时时间
 * @retval  uint8_t 1:超时, 0:未超时
 */
uint8_t  dwt_is_timeout(uint32_t start, uint32_t timeout_us);

#endif
