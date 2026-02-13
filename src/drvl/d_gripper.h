/**
 * @file  d_gripper.h
 * @brief 夹爪驱动 (OOP 封装)
 *        通过 USART2 向舵机发送 PWM 指令
 */
#ifndef _d_gripper_h_
#define _d_gripper_h_

#include "stm32f10x.h"
#include "usart.h"
#include <stdint.h>

// ! ========================= 接 口 变 量 / Typedef 声 明 ========================= ! //

typedef struct Gripper Gripper;
struct Gripper {
    /* public methods */
    void (*init)(Gripper* self, Usart* uart);
    void (*open)(Gripper* self);
    void (*close)(Gripper* self);
    void (*set_pos)(Gripper* self, uint16_t pulse);
    /* private data */
    Usart* uart_;
};

// ! ========================= 接 口 函 数 声 明 ========================= ! //

/**
 * @brief   创建 Gripper 对象
 * @param   None
 * @retval  Gripper 对象
 */
Gripper gripper_create(void);

#endif
