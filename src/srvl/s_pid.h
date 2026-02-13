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
    /* public methods */
    void  (*init)(Pid* self);
    void  (*set_pid)(Pid* self, float p, float i, float d);
    void  (*set_limit)(Pid* self, float int_limit, float out_limit);
    void  (*set_filter)(Pid* self, float alpha, float int_threshold, float dead_zone);
    float (*calculate)(Pid* self, float target, float actual, float dt_s);
    /* private data */
    float p_;
    float i_;
    float d_;
    float output_;
    float integral_;
    float last_actual_;
    float last_diff_;
    float alpha_;
    float int_limit_;
    float out_limit_;
    float int_threshold_;
    float dead_zone_;
};

// ! ========================= 接 口 函 数 声 明 ========================= ! //

/**
 * @brief   创建 Pid 对象
 * @param   None
 * @retval  Pid 对象
 */
Pid pid_create(void);

#endif
