/**
 * @file  s_pid.h
 * @brief PID 控制器 (OOP 封装)
 */
#ifndef _s_pid_h_
#define _s_pid_h_

#include <stdint.h>

// ! ========================= 接 口 变 量 / Typedef 声 明 ========================= ! //

typedef struct Pid Pid;
struct Pid {
// public:
    /**
     * @brief   初始化 PID
     * @param   self PID对象
     * @retval  None
     */
    void  (*init)(Pid* self);
    /**
     * @brief   设置 PID 参数
     * @param   self PID对象
     * @param   p 比例系数
     * @param   i 积分系数
     * @param   d 微分系数
     * @retval  None
     */
    void  (*set_pid)(Pid* self, float p, float i, float d);
    /**
     * @brief   设置限幅
     * @param   self PID对象
     * @param   int_limit 积分限幅
     * @param   out_limit 输出限幅
     * @retval  None
     */
    void  (*set_limit)(Pid* self, float int_limit, float out_limit);
    /**
     * @brief   设置滤波和其他参数
     * @param   self PID对象
     * @param   alpha 滤波系数(0-1)
     * @param   int_threshold 积分分离阈值
     * @param   dead_zone 获取不到区域
     * @retval  None
     */
    void  (*set_filter)(Pid* self, float alpha, float int_threshold, float dead_zone);
    /**
     * @brief   计算 PID 输出
     * @param   self PID对象
     * @param   target 目标值
     * @param   actual 实际值
     * @param   dt_s 时间间隔(秒)
     * @retval  float 输出值
     */
    float (*calculate)(Pid* self, float target, float actual, float dt_s);

// private:
    float _p_;
    float _i_;
    float _d_;
    float _output_;
    float _integral_;
    float _last_actual_;
    float _last_diff_;
    float _alpha_;
    float _int_limit_;
    float _out_limit_;
    float _int_threshold_;
    float _dead_zone_;
};

// ! ========================= 接 口 函 数 声 明 ========================= ! //

Pid pid_create(void);

#endif
