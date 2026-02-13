/**
 * @file  s_lift_control.c
 * @brief 升降台控制服务实现
 *        手动 / 自动 (目标高度差) 两种模式
 */
#include "s_lift_control.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

// ! ========================= 变 量 声 明 ========================= ! //

#define POSITION_TOLERANCE  5   /* 到位误差 (mm) */

// ! ========================= 私 有 函 数 声 明 ========================= ! //

static void _init(LiftControl* self, Encoder* enc, LiftMotor* motor);
static void _set_height(LiftControl* self, float delta_mm);
static void _manual(LiftControl* self, LiftDir_e dir);
static void _update(LiftControl* self);

// ! ========================= 接 口 函 数 实 现 ========================= ! //

/**
 * @brief   创建 LiftControl 对象
 * @param   None
 * @retval  LiftControl 对象
 */
LiftControl lift_ctrl_create(void) {
    LiftControl obj;
    obj.encoder_ = 0;
    obj.motor_ = 0;
    obj.target_pos_ = 0;
    obj.height_diff_ = 0.0f;
    obj.rcvd_flag_ = 0;
    obj.manual_dir_ = LiftDirStop_e;
    obj.init = _init;
    obj.set_height = _set_height;
    obj.manual = _manual;
    obj.update = _update;
    return obj;
}

// ! ========================= 私 有 函 数 实 现 ========================= ! //

/**
 * @brief   初始化控制模块
 * @param   self 升降机控制器对象
 * @param   enc 编码器对象
 * @param   motor 电机对象
 * @retval  None
 */
static void _init(LiftControl* self, Encoder* enc, LiftMotor* motor) {
    self->encoder_ = enc;
    self->motor_ = motor;
    self->target_pos_ = 0;
    self->height_diff_ = 0.0f;
    self->rcvd_flag_ = 0;
    self->manual_dir_ = LiftDirStop_e;
}

/**
 * @brief   设定目标高度差
 * @param   self 升降机控制器对象
 * @param   delta_mm 高度差(mm)
 * @retval  None
 */
static void _set_height(LiftControl* self, float delta_mm) {
    self->height_diff_ = delta_mm;
    self->rcvd_flag_ = 1;
}

/**
 * @brief   手动控制命令
 * @param   self 升降机控制器对象
 * @param   dir 方向
 * @retval  None
 */
static void _manual(LiftControl* self, LiftDir_e dir) {
    self->manual_dir_ = dir;
}

/**
 * @brief   控制循环更新
 * @param   self 升降机控制器对象
 * @retval  None
 */
static void _update(LiftControl* self) {
    /* 更新编码器 */
    self->encoder_->update(self->encoder_);
    int32_t pos = self->encoder_->get_position(self->encoder_);

    /* 收到新的高度目标 */
    if(self->rcvd_flag_) {
        self->target_pos_ = pos + (int32_t)self->height_diff_;
        self->rcvd_flag_ = 0;
    }

    /* 手动模式优先 */
    if(self->manual_dir_ != LiftDirStop_e) {
        self->motor_->set_dir(self->motor_, self->manual_dir_);
        return;
    }

    /* 自动定位 */
    if(self->height_diff_ != 0.0f) {
        int32_t error = self->target_pos_ - pos;
        if(abs(error) > POSITION_TOLERANCE) {
            self->motor_->set_dir(self->motor_,
                error > 0 ? LiftDirUp_e : LiftDirDown_e);
        }
        else {
            self->motor_->stop(self->motor_);
            printf("$LIFTER:OK#\r\n");
            self->height_diff_ = 0.0f;
        }
    }
    else {
        self->motor_->stop(self->motor_);
    }
}
