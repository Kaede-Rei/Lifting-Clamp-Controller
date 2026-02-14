/**
 * @file  s_pid.c
 * @brief PID 控制器实现
 */
#include "s_pid.h"

// ! ========================= 变 量 声 明 ========================= ! //

#define CONSTRAIN(val, lo, hi) ((val) > (hi) ? (hi) : ((val) < (lo) ? (lo) : (val)))

// ! ========================= 私 有 函 数 声 明 ========================= ! //

static void _init(Pid* self);
static void _set_pid(Pid* self, float p, float i, float d);
static void _set_limit(Pid* self, float int_limit, float out_limit);
static void _set_filter(Pid* self, float alpha, float int_threshold, float dead_zone);
static float _calculate(Pid* self, float target, float actual, float dt_s);

// ! ========================= 接 口 函 数 实 现 ========================= ! //

/**
 * @brief   创建 Pid 对象
 * @param   None
 * @retval  Pid 对象
 */
Pid pid_create(void) {
    Pid obj;
    obj._p_ = obj._i_ = obj._d_ = 0.0f;
    obj._output_ = 0.0f;
    obj._integral_ = 0.0f;
    obj._last_actual_ = 0.0f;
    obj._last_diff_ = 0.0f;
    obj._alpha_ = 1.0f;
    obj._int_limit_ = 0.0f;
    obj._out_limit_ = 0.0f;
    obj._int_threshold_ = 0.0f;
    obj._dead_zone_ = 0.0f;
    obj.init = _init;
    obj.set_pid = _set_pid;
    obj.set_limit = _set_limit;
    obj.set_filter = _set_filter;
    obj.calculate = _calculate;
    return obj;
}

// ! ========================= 私 有 函 数 实 现 ========================= ! //

/**
 * @brief   初始化 PID
 * @param   self PID对象
 * @retval  None
 */
static void _init(Pid* self) {
    self->_p_ = 0.0f;
    self->_i_ = 0.0f;
    self->_d_ = 0.0f;
    self->_output_ = 0.0f;
    self->_integral_ = 0.0f;
    self->_last_actual_ = 0.0f;
    self->_last_diff_ = 0.0f;
    self->_alpha_ = 1.0f;
    self->_int_limit_ = 0.0f;
    self->_out_limit_ = 0.0f;
    self->_int_threshold_ = 0.0f;
    self->_dead_zone_ = 0.0f;
}

/**
 * @brief   设置 PID 参数
 * @param   self PID对象
 * @param   p 比例系数
 * @param   i 积分系数
 * @param   d 微分系数
 * @retval  None
 */
static void _set_pid(Pid* self, float p, float i, float d) {
    self->_p_ = p;
    self->_i_ = i;
    self->_d_ = d;
}

/**
 * @brief   设置限幅
 * @param   self PID对象
 * @param   int_limit 积分限幅
 * @param   out_limit 输出限幅
 * @retval  None
 */
static void _set_limit(Pid* self, float int_limit, float out_limit) {
    self->_int_limit_ = int_limit;
    self->_out_limit_ = out_limit;
}

/**
 * @brief   设置滤波和其他参数
 * @param   self PID对象
 * @param   alpha 滤波系数(0-1)
 * @param   int_threshold 积分分离阈值
 * @param   dead_zone 获取不到区域
 * @retval  None
 */
static void _set_filter(Pid* self, float alpha, float int_threshold, float dead_zone) {
    self->_alpha_ = alpha;
    self->_int_threshold_ = int_threshold;
    self->_dead_zone_ = dead_zone;
}

/**
 * @brief   计算 PID 输出
 * @param   self PID对象
 * @param   target 目标值
 * @param   actual 实际值
 * @param   dt_s 时间间隔(秒)
 * @retval  float 输出值
 */
static float _calculate(Pid* self, float target, float actual, float dt_s) {
    if(dt_s <= 1e-6f) dt_s = 1e-6f;
    else if(dt_s > 0.1f) dt_s = 0.1f;

    float err = target - actual;
    if(err < self->_dead_zone_ && err > -self->_dead_zone_) {
        self->_output_ = 0.0f;
        return 0.0f;
    }

    /* 微分项 (基于测量值求导, 避免目标突变) */
    float diff = -(actual - self->_last_actual_) / dt_s;
    self->_last_actual_ = actual;
    float filtered = self->_alpha_ * diff + (1.0f - self->_alpha_) * self->_last_diff_;
    self->_last_diff_ = filtered;

    /* 积分项 */
    self->_integral_ += err * dt_s;
    self->_integral_ = CONSTRAIN(self->_integral_, -self->_int_limit_, self->_int_limit_);
    if(self->_int_threshold_ > 0.0f) {
        if(err > self->_int_threshold_ || err < -self->_int_threshold_)
            self->_integral_ = 0.0f;
    }

    /* PID 输出 */
    self->_output_ = self->_p_ * err
        + self->_i_ * self->_integral_
        + self->_d_ * filtered;
    self->_output_ = CONSTRAIN(self->_output_, -self->_out_limit_, self->_out_limit_);
    return self->_output_;
}
