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
// public:
    /**
     * @brief   初始化 CAN
     * @param   self CAN对象
     * @retval  None
     */
    void (*init)(Can* self);
    /**
     * @brief   发送 CAN 报文
     * @param   self CAN对象
     * @param   std_id 标准ID
     * @param   data 数据指针
     * @param   len 数据长度
     * @retval  bool 是否发送成功
     */
    bool (*send)(Can* self, uint16_t std_id, const uint8_t* data, uint8_t len);
    /**
     * @brief   设置接收回调
     * @param   self CAN对象
     * @param   cb 回调函数
     * @retval  None
     */
    void (*set_rx_cb)(Can* self, CanRxCb cb);

// private:
    CanMode_e _mode_;
    CanRxCb _rx_cb_;
};

// ! ========================= 接 口 函 数 声 明 ========================= ! //

Can can_create(CanMode_e mode);

#endif
