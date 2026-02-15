/**
 * @file  main.c
 * @brief 程序入口
 */
#include "a_board.h"

int main(void) {
    a_board_init();

    while(1) {
        fsm_process();

        if(tick.flag) {
            tick.flag = 0;
            lift_encoder.update(&lift_encoder);
        }
    }
}
