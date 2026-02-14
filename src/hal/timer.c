/**
 * @file  timer.c
 * @brief TIM3 周期定时器实现
 *        72 MHz / 720 / 1000 = 10 ms
 */
#include "timer.h"

// ! ========================= 变 量 声 明 ========================= ! //

static Timer* _instance = 0;

// ! ========================= 私 有 函 数 声 明 ========================= ! //

static void _init(Timer* self);
static void _set_callback(Timer* self, TimerCb cb);

// ! ========================= 接 口 函 数 实 现 ========================= ! //

/**
 * @brief   创建 Timer 对象
 * @param   None
 * @retval  Timer 对象
 */
Timer timer_create(void) {
    Timer obj;
    obj.flag_ = 0;
    obj._callback_ = 0;
    obj.init = _init;
    obj.set_callback = _set_callback;
    return obj;
}

/**
 * @brief   TIM3 中断服务
 * @param   None
 * @retval  None
 */
void TIM3_IRQHandler(void) {
    if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update) == SET) {
        if(_instance) {
            _instance->flag_ = 1;
            if(_instance->_callback_)
                _instance->_callback_();
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

// ! ========================= 私 有 函 数 实 现 ========================= ! //

/**
 * @brief   初始化定时器
 * @param   self 定时器对象
 * @retval  None
 */
static void _init(Timer* self) {
    _instance = self;
    self->flag_ = 0;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_InternalClockConfig(TIM3);

    TIM_TimeBaseInitTypeDef tb;
    tb.TIM_ClockDivision = TIM_CKD_DIV1;
    tb.TIM_CounterMode = TIM_CounterMode_Up;
    tb.TIM_Period = 1000 - 1;        /* 10 ms */
    tb.TIM_Prescaler = 720 - 1;
    tb.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &tb);

    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef ni;
    ni.NVIC_IRQChannel = TIM3_IRQn;
    ni.NVIC_IRQChannelCmd = ENABLE;
    ni.NVIC_IRQChannelPreemptionPriority = 1;
    ni.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&ni);

    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief   设置回调函数
 * @param   self 定时器对象
 * @param   cb 回调函数
 * @retval  None
 */
static void _set_callback(Timer* self, TimerCb cb) {
    self->_callback_ = cb;
}
