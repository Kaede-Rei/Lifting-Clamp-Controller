/**
 * @file  main.c
 * @brief 程序入口
 */
#include "a_board.h"

int main(void) {
    a_board_init();

    while(1) {
        if(tick.flag_) {
            tick.flag_ = 0;

            comms.process(&comms);
            lift_ctrl.update(&lift_ctrl);
        }
    }
}
