/**
 * @file  s_comms.h
 * @brief 无线串口通信协议服务 (OOP 封装)
 *        通过 USART1 接收工控机指令, 控制升降台与夹爪
 */
#ifndef _s_comms_h_
#define _s_comms_h_

#include "usart.h"
#include "s_lift_control.h"
#include "d_gripper.h"
#include <stdint.h>

// ! ========================= 接 口 变 量 / Typedef 声 明 ========================= ! //

#define COMMS_CMD_BUF_SIZE  64

typedef struct Comms Comms;
struct Comms {
    /* public methods */
    void (*init)(Comms* self, Usart* uart, LiftControl* lift, Gripper* grip);
    void (*process)(Comms* self);
    /* private data */
    Usart* uart_;
    LiftControl* lift_;
    Gripper* grip_;
    char         cmd_buf_[COMMS_CMD_BUF_SIZE];
    uint8_t      cmd_idx_;
    uint8_t      parsing_;
};

// ! ========================= 接 口 函 数 声 明 ========================= ! //

/**
 * @brief   创建 Comms 对象
 * @param   None
 * @retval  Comms 对象
 */
Comms comms_create(void);

#endif
