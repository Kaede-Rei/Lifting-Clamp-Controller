#ifndef _fsm_h_
#define _fsm_h_

// ! ========================= 接 口 变 量 / Typedef 声 明 ========================= ! //

/**
 * @brief   事件枚举
 */
typedef enum {
    EVENT_NONE = 0,
    EVENT_LIFT_MOVE,
    EVENT_LIFT_STOP,
    EVENT_MAX
} event_e;

/**
 * @brief   状态结构体
 */
typedef struct State State;
struct State {
    /**
     * @brief   事件处理函数
     * @param   event 事件
     */
    State* (*handle_event)(event_e event);
    /**
     * @brief   状态持续动作函数
     */
    void(*action)(void);
    /**
     * @brief   状态进入动作函数
     */
    void(*entry)(void);
    /**
     * @brief   状态退出动作函数
     */
    void(*exit)(void);
};

extern event_e cur_event;
extern State state_idle, state_lift_moving;

// ! ========================= 接 口 函 数 声 明 ========================= ! //

void fsm_process(void);
void fsm_trigger_event(event_e e);

#endif
