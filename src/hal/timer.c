/**
 * @file  timer.c
 * @brief 通用定时器 HAL 实现 — 配置表驱动
 *        根据 tim_cfg_t 自动适配 TIM1 ~ TIM4
 */
#include "timer.h"

// ! ========================= 变 量 声 明 ========================= ! //

typedef struct {
    TIM_TypeDef* periph;
    uint32_t rcc_mask;
    uint8_t rcc_bus;       /* 1 = APB1, 2 = APB2 */
    uint8_t irqn;
} tim_hw_t;

static const tim_hw_t _hw[TIM_COUNT] = {
    [TIM_1] = {.periph = TIM1,
            .rcc_mask = RCC_APB2Periph_TIM1,
            .rcc_bus = 2,
            .irqn = TIM1_UP_IRQn },
    [TIM_2] = {.periph = TIM2,
            .rcc_mask = RCC_APB1Periph_TIM2,
            .rcc_bus = 1,
            .irqn = TIM2_IRQn },
    [TIM_3] = {.periph = TIM3,
            .rcc_mask = RCC_APB1Periph_TIM3,
            .rcc_bus = 1,
            .irqn = TIM3_IRQn },
    [TIM_4] = {.periph = TIM4,
            .rcc_mask = RCC_APB1Periph_TIM4,
            .rcc_bus = 1,
            .irqn = TIM4_IRQn },
};

static tim_t* _handles[TIM_COUNT] = { 0 };

// ! ========================= 私 有 函 数 声 明 ========================= ! //



// ! ========================= 接 口 函 数 实 现 ========================= ! //

/**
 * @brief   初始化定时器 (依据配置表)
 * @param   handle 句柄
 * @param   cfg 配置表
 */
void tim_init(tim_t* handle, const tim_cfg_t* cfg) {
    handle->cfg = cfg;
    handle->flag = 0;
    handle->callback = 0;

    tim_id_e id = cfg->id;
    const tim_hw_t* hw = &_hw[id];
    _handles[id] = handle;

    /* 时钟使能 */
    if(hw->rcc_bus == 2)
        RCC_APB2PeriphClockCmd(hw->rcc_mask, ENABLE);
    else
        RCC_APB1PeriphClockCmd(hw->rcc_mask, ENABLE);

    TIM_InternalClockConfig(hw->periph);

    /* 时基 */
    TIM_TimeBaseInitTypeDef tb;
    tb.TIM_ClockDivision = TIM_CKD_DIV1;
    tb.TIM_CounterMode = TIM_CounterMode_Up;
    tb.TIM_Period = cfg->period;
    tb.TIM_Prescaler = cfg->prescaler;
    tb.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(hw->periph, &tb);

    TIM_ClearFlag(hw->periph, TIM_FLAG_Update);
    TIM_ITConfig(hw->periph, TIM_IT_Update, ENABLE);

    /* NVIC */
    NVIC_InitTypeDef ni;
    ni.NVIC_IRQChannel = hw->irqn;
    ni.NVIC_IRQChannelCmd = ENABLE;
    ni.NVIC_IRQChannelPreemptionPriority = cfg->nvic_preempt;
    ni.NVIC_IRQChannelSubPriority = cfg->nvic_sub;
    NVIC_Init(&ni);

    TIM_Cmd(hw->periph, ENABLE);
}

/**
 * @brief   设置定时器中断回调
 * @param   handle 句柄
 * @param   cb 回调函数
 */
void tim_set_callback(tim_t* handle, tim_cb_t cb) {
    handle->callback = cb;
}

// ! ========================= 私 有 函 数 实 现 ========================= ! //

/**
 * @brief   定时器中断服务函数
 * @param   id 定时器 ID
 * @note    由 TIM1_UP_IRQHandler、TIM2_IRQHandler、TIM3_IRQHandler、TIM4_IRQHandler 调用
 */
static void _tim_irq(tim_id_e id) {
    tim_t* handle = _handles[id];
    if(!handle) return;
    const tim_hw_t* hw = &_hw[id];
    if(TIM_GetFlagStatus(hw->periph, TIM_FLAG_Update) == SET) {
        handle->flag = 1;
        if(handle->callback) handle->callback();
        TIM_ClearITPendingBit(hw->periph, TIM_IT_Update);
    }
}

void TIM1_UP_IRQHandler(void) { _tim_irq(TIM_1); }
void TIM2_IRQHandler(void) { _tim_irq(TIM_2); }
void TIM3_IRQHandler(void) { _tim_irq(TIM_3); }
void TIM4_IRQHandler(void) { _tim_irq(TIM_4); }
