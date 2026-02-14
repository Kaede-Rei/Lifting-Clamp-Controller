/**
 * @file  can.c
 * @brief CAN1 外设驱动实现
 *        PA11-RX  PA12-TX  1 Mbps Classic
 */
#include "can.h"

// ! ========================= 变 量 声 明 ========================= ! //

static Can* _instance = 0;

// ! ========================= 私 有 函 数 声 明 ========================= ! //

static void _init(Can* self);
static bool _send(Can* self, uint16_t std_id, const uint8_t* data, uint8_t len);
static void _set_rx_cb(Can* self, CanRxCb cb);

// ! ========================= 接 口 函 数 实 现 ========================= ! //

/**
 * @brief   创建 Can 对象
 * @param   mode CAN模式
 * @retval  Can 对象
 */
Can can_create(CanMode_e mode) {
    Can obj;
    obj._mode_ = mode;
    obj._rx_cb_ = 0;
    obj.init = _init;
    obj.send = _send;
    obj.set_rx_cb = _set_rx_cb;
    return obj;
}

/**
 * @brief   CAN1 接收中断服务
 * @param   None
 * @retval  None
 */
void USB_LP_CAN1_RX0_IRQHandler(void) {
    if(CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET) {
        CanRxMsg rx;
        CAN_Receive(CAN1, CAN_FIFO0, &rx);
        if(_instance && _instance->_rx_cb_) {
            _instance->_rx_cb_(&rx);
        }
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
    }
}

// ! ========================= 私 有 函 数 实 现 ========================= ! //

/**
 * @brief   初始化 CAN
 * @param   self CAN对象
 * @retval  None
 */
static void _init(Can* self) {
    _instance = self;

    /* GPIO: PA12=TX(AF_PP), PA11=RX(IPU) */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Pin = GPIO_Pin_12;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &gpio);

    /* CAN 基本配置 */
    CAN_InitTypeDef ci;
    ci.CAN_TTCM = DISABLE;
    ci.CAN_ABOM = ENABLE;
    ci.CAN_AWUM = DISABLE;
    ci.CAN_NART = DISABLE;
    ci.CAN_RFLM = DISABLE;
    ci.CAN_TXFP = DISABLE;

    switch(self->_mode_) {
        case CanModeNormal_e:         ci.CAN_Mode = CAN_Mode_Normal;          break;
        case CanModeLoopback_e:       ci.CAN_Mode = CAN_Mode_LoopBack;        break;
        case CanModeSilent_e:         ci.CAN_Mode = CAN_Mode_Silent;          break;
        case CanModeSilentLoopback_e: ci.CAN_Mode = CAN_Mode_Silent_LoopBack; break;
    }

    /* 1 Mbps: APB1=36 MHz, Prescaler=4, BS1=7tq, BS2=1tq
     * Baud = 36 MHz / (4 * (1 + 7 + 1)) = 1 MHz */
    ci.CAN_SJW = CAN_SJW_1tq;
    ci.CAN_BS1 = CAN_BS1_7tq;
    ci.CAN_BS2 = CAN_BS2_1tq;
    ci.CAN_Prescaler = 4;
    CAN_Init(CAN1, &ci);

    /* 滤波器: 全部接收 */
    CAN_FilterInitTypeDef fi;
    fi.CAN_FilterNumber = 0;
    fi.CAN_FilterMode = CAN_FilterMode_IdMask;
    fi.CAN_FilterScale = CAN_FilterScale_32bit;
    fi.CAN_FilterIdHigh = 0x0000;
    fi.CAN_FilterIdLow = 0x0000;
    fi.CAN_FilterMaskIdHigh = 0x0000;
    fi.CAN_FilterMaskIdLow = 0x0000;
    fi.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
    fi.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&fi);

    /* RX0 中断 */
    NVIC_InitTypeDef ni;
    ni.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    ni.NVIC_IRQChannelPreemptionPriority = 1;
    ni.NVIC_IRQChannelSubPriority = 0;
    ni.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&ni);
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

/**
 * @brief   发送 CAN 报文
 * @param   self CAN对象
 * @param   std_id 标准ID
 * @param   data 数据指针
 * @param   len 数据长度
 * @retval  bool 是否发送成功
 */
static bool _send(Can* self, uint16_t std_id, const uint8_t* data, uint8_t len) {
    (void)self;
    if(len > 8) return false;

    CanTxMsg tx;
    tx.StdId = std_id & 0x7FF;
    tx.ExtId = 0;
    tx.IDE = CAN_ID_STD;
    tx.RTR = CAN_RTR_DATA;
    tx.DLC = len;
    for(uint8_t i = 0; i < len; ++i)
        tx.Data[i] = data[i];

    uint8_t mbox = CAN_Transmit(CAN1, &tx);
    if(mbox == CAN_TxStatus_NoMailBox) return false;

    uint32_t timeout = 0;
    while(CAN_TransmitStatus(CAN1, mbox) != CAN_TxStatus_Ok) {
        if(++timeout > 0xFFFF) return false;
    }
    return true;
}

/**
 * @brief   设置接收回调
 * @param   self CAN对象
 * @param   cb 回调函数
 * @retval  None
 */
static void _set_rx_cb(Can* self, CanRxCb cb) {
    self->_rx_cb_ = cb;
}
