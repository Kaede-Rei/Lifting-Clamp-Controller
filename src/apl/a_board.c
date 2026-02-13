/**
 * @file  a_board.c
 * @brief 板级初始化实现
 *        按 HAL → DRVL → SRVL 顺序创建并初始化所有模块
 */
#include "a_board.h"
#include "main.h"

// ! ========================= 变 量 声 明 ========================= ! //

Can         can;
Usart       usart1;
Usart       usart2;
Timer       tick;
Encoder     encoder;
LiftMotor   lift_motor;
Gripper     gripper;
Comms       comms;
LiftControl lift_ctrl;

// ! ========================= 私 有 函 数 声 明 ========================= ! //



// ! ========================= 接 口 函 数 实 现 ========================= ! //

/**
 * @brief   板级初始化函数
 * @param   None
 * @retval  None
 */
void a_board_init(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* ---------- 底层时基 ---------- */
    systick_init();
    dwt_init();

    /* ---------- 创建对象 ---------- */
    can = can_create(CanModeNormal_e);
    usart1 = usart1_create(USART1_BAUD);
    usart2 = usart2_create(USART2_BAUD);
    tick = timer_create();
    encoder = encoder_create();
    lift_motor = lift_motor_create();
    gripper = gripper_create();
    comms = comms_create();
    lift_ctrl = lift_ctrl_create();

    /* ---------- HAL 初始化 ---------- */
    can.init(&can);
    usart1.init(&usart1);
    usart2.init(&usart2);
    tick.init(&tick);

    /* ---------- 驱动初始化 ---------- */
    encoder.init(&encoder);
    lift_motor.init(&lift_motor);
    gripper.init(&gripper, &usart2);

    /* ---------- 服务初始化 ---------- */
    comms.init(&comms, &usart1, &lift_ctrl, &gripper);
    lift_ctrl.init(&lift_ctrl, &encoder, &lift_motor);

    s_delay_ms(1000);
    printf("Board initialized!\r\n");
}

// ! ========================= 私 有 函 数 实 现 ========================= ! //


