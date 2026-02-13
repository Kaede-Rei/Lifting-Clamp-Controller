/**
 * @file  s_lift_control.h
 * @brief 升降台控制服务 (OOP 封装)
 *        整合编码器反馈与电机控制
 */
#ifndef _s_lift_control_h_
#define _s_lift_control_h_

#include "d_encoder.h"
#include "d_lift_motor.h"
#include <stdint.h>

// ! ========================= 接 口 变 量 / Typedef 声 明 ========================= ! //

typedef struct LiftControl LiftControl;
struct LiftControl {
    /* public methods */
    void (*init)(LiftControl* self, Encoder* enc, LiftMotor* motor);
    void (*set_height)(LiftControl* self, float delta_mm);
    void (*manual)(LiftControl* self, LiftDir_e dir);
    void (*update)(LiftControl* self);
    /* private data */
    Encoder* encoder_;
    LiftMotor* motor_;
    int32_t    target_pos_;
    float      height_diff_;
    uint8_t    rcvd_flag_;
    LiftDir_e  manual_dir_;
};

// ! ========================= 接 口 函 数 声 明 ========================= ! //

/**
 * @brief   创建 LiftControl 对象
 * @param   None
 * @retval  LiftControl 对象
 */
LiftControl lift_ctrl_create(void);

#endif
