/**
 * @file  s_pid.c
 * @brief PID 控制器实现
 */
#include "s_pid.h"

// ! ========================= 变 量 声 明 ========================= ! //

#define CONSTRAIN(val, lo, hi) ((val) > (hi) ? (hi) : ((val) < (lo) ? (lo) : (val)))

// ! ========================= 私 有 函 数 声 明 ========================= ! //

static void  _init(Pid* self);
static void  _set_pid(Pid* self, float p, float i, float d);
static void  _set_limit(Pid* self, float int_limit, float out_limit);
static void  _set_filter(Pid* self, float alpha, float int_threshold, float dead_zone);
static float _calculate(Pid* self, float target, float actual, float dt_s);

// ! ========================= 接 口 函 数 实 现 ========================= ! //

/**
 * @brief   创建 Pid 对象
 * @param   None
 * @retval  Pid 对象
 */
Pid pid_create(void) {
    Pid obj;
    obj.p_ = obj.i_ = obj.d_ = 0.0f;
    obj.output_ = 0.0f;
    obj.integral_ = 0.0f;
    obj.last_actual_ = 0.0f;
    obj.last_diff_ = 0.0f;
    obj.alpha_ = 1.0f;
    obj.int_limit_ = 0.0f;
    obj.out_limit_ = 0.0f;
    obj.int_threshold_ = 0.0f;
    obj.dead_zone_ = 0.0f;
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
    self->p_ = 0.0f;
    self->i_ = 0.0f;
    self->d_ = 0.0f;
    self->output_ = 0.0f;
    self->integral_ = 0.0f;
    self->last_actual_ = 0.0f;
    self->last_diff_ = 0.0f;
    self->alpha_ = 1.0f;
    self->int_limit_ = 0.0f;
    self->out_limit_ = 0.0f;
    self->int_threshold_ = 0.0f;
    self->dead_zone_ = 0.0f;
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
    self->p_ = p;
    self->i_ = i;
    self->d_ = d;
}

/**
 * @brief   设置限幅
 * @param   self PID对象
 * @param   int_limit 积分限幅
 * @param   out_limit 输出限幅
 * @retval  None
 */
static void _set_limit(Pid* self, float int_limit, float out_limit) {
    self->int_limit_ = int_limit;
    self->out_limit_ = out_limit;
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
    self->alpha_ = alpha;
    self->int_threshold_ = int_threshold;
    self->dead_zone_ = dead_zone;
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
    if(err < self->dead_zone_ && err > -self->dead_zone_) {
        self->output_ = 0.0f;
        return 0.0f;
    }

    /* 微分项 (基于测量值求导, 避免目标突变) */
    float diff = -(actual - self->last_actual_) / dt_s;
    self->last_actual_ = actual;
    float filtered = self->alpha_ * diff + (1.0f - self->alpha_) * self->last_diff_;
    self->last_diff_ = filtered;

    /* 积分项 */
    self->integral_ += err * dt_s;
    self->integral_ = CONSTRAIN(self->integral_, -self->int_limit_, self->int_limit_);
    if(self->int_threshold_ > 0.0f) {
        if(err > self->int_threshold_ || err < -self->int_threshold_)
            self->integral_ = 0.0f;
    }

    /* PID 输出 */
    self->output_ = self->p_ * err
        + self->i_ * self->integral_
        + self->d_ * filtered;
    self->output_ = CONSTRAIN(self->output_, -self->out_limit_, self->out_limit_);
    return self->output_;
}
