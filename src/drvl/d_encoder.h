/**
 * @file  d_encoder.h
 * @brief 编码器驱动 (OOP 封装)
 *        TIM2 编码器模式, PA0=CH1, PA1=CH2
 *        读取脉冲并换算为毫米位移与速度
 */
#ifndef _d_encoder_h_
#define _d_encoder_h_

#include "stm32f10x.h"
#include <stdint.h>

// ! ========================= 接 口 变 量 / Typedef 声 明 ========================= ! //

typedef struct Encoder Encoder;
struct Encoder {
    /* public methods */
    void    (*init)(Encoder* self);
    void    (*update)(Encoder* self);
    int32_t(*get_position)(const Encoder* self);
    int32_t(*get_speed)(const Encoder* self);
    /* private data */
    int32_t total_pulses_;
    int32_t position_mm_;
    int32_t speed_;
};

// ! ========================= 接 口 函 数 声 明 ========================= ! //

/**
 * @brief   创建 Encoder 对象
 * @param   None
 * @retval  Encoder 对象
 */
Encoder encoder_create(void);

#endif
