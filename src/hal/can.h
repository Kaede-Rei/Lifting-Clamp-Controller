/**
 * @file  can.h
 * @brief CAN1 外设驱动
 *        PA11-RX  PA12-TX  1 Mbps Classic
 */
#ifndef _can_h_
#define _can_h_

#include "stm32f10x.h"
#include <stdbool.h>
#include <stdint.h>

// ! ========================= 接 口 变 量 / Typedef 声 明 ========================= ! //

typedef enum {
    CanModeNormal_e = 0,
    CanModeLoopback_e,
    CanModeSilent_e,
    CanModeSilentLoopback_e
} CanMode_e;

typedef void (*CanRxCb)(CanRxMsg* msg);

typedef struct Can Can;
struct Can {
    /* public methods */
    void    (*init)(Can* self);
    bool    (*send)(Can* self, uint16_t std_id, const uint8_t* data, uint8_t len);
    void    (*set_rx_cb)(Can* self, CanRxCb cb);
    /* private data */
    CanMode_e mode_;
    CanRxCb   rx_cb_;
};

// ! ========================= 接 口 函 数 声 明 ========================= ! //

/**
 * @brief   创建 Can 对象
 * @param   mode CAN模式
 * @retval  Can 对象
 */
Can can_create(CanMode_e mode);

#endif
