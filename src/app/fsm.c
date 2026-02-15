#include "fsm.h"
#include "a_board.h"

#include <math.h>

// ! ========================= 变 量 声 明 ========================= ! //

event_e cur_event = EVENT_NONE;
State* cur_state = &state_idle;

// ! ========================= 私 有 函 数 声 明 ========================= ! //

/**
 * @brief   事件处理函数
 * @param   e 事件
 */
static State* handle_event(event_e e);

/**
 * @brief   空闲状态
 */
static void idle_action(void);
State state_idle = {
    .handle_event = handle_event,
    .action = idle_action,
    .entry = 0,
    .exit = 0,
};

/**
 * @brief   升降台移动状态
 */
static void lift_moving_action(void);
State state_lift_moving = {
    .handle_event = handle_event,
    .action = lift_moving_action,
    .entry = 0,
    .exit = 0,
};

// ! ========================= 接 口 函 数 实 现 ========================= ! //

/**
 * @brief   FSM 处理函数
 */
void fsm_process(void) {
    // 根据当前事件和状态获取下一个状态
    State* next_state = cur_state->handle_event(cur_event);
    if(next_state != cur_state) {
        // 当前状态退出动作
        if(cur_state->exit) {
            cur_state->exit();
        }

        // 状态转移
        cur_state = next_state;

        // 新状态进入动作
        if(cur_state->entry) {
            cur_state->entry();
        }
    }

    // 状态持续动作
    if(cur_state->action) {
        cur_state->action();
    }
}

/**
 * @brief   触发事件
 * @param   e 事件
 */
void fsm_trigger_event(event_e e) {
    cur_event = e;
}

// ! ========================= 私 有 函 数 实 现 ========================= ! //

/**
 * @brief   事件处理函数
 * @param   e 事件
 */
static State* handle_event(event_e e) {
    switch(e) {
        case EVENT_LIFT_MOVE:
            return &state_lift_moving;
        case EVENT_LIFT_STOP:
            return &state_idle;
        default:
            return cur_state;
    }
}

/**
 * @brief   空闲状态动作函数
 */
static void idle_action(void) {
    if(fabsf(lift_target_pos_mm - lift_encoder.get_position(&lift_encoder)) > 5.0f) {
        fsm_trigger_event(EVENT_LIFT_MOVE);
    }
}

/**
 * @brief   升降台移动状态动作函数
 */
static void lift_moving_action(void) {
    float target = lift_target_pos_mm;
    float current = lift_encoder.get_position(&lift_encoder);

    if(target - current > 5.0f) {
        lift_relay.set_dir(&lift_relay, RelayDirA);
    }
    else if(target - current < -5.0f) {
        lift_relay.set_dir(&lift_relay, RelayDirB);
    }
    else {
        lift_relay.stop(&lift_relay);
        fsm_trigger_event(EVENT_LIFT_STOP);
    }
}
