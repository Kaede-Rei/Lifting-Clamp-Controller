/**
 * @file  s_log.h
 * @brief 日志输出服务
 */
#ifndef _s_log_h_
#define _s_log_h_

#include <stdio.h>
#include <stdarg.h>

// ! ========================= 接 口 变 量 / Typedef 声 明 ========================= ! //

/* ========================= 日志等级 ========================= */

#define LOG_LEVEL_ERROR  1
#define LOG_LEVEL_WARN   2
#define LOG_LEVEL_INFO   3

#ifndef LOG_LEVEL
#define LOG_LEVEL  LOG_LEVEL_INFO
#endif

// ! ========================= 接 口 函 数 声 明 ========================= ! //

/**
 * @brief   输出波形数据
 * @param   count 数据个数
 * @param   ... float* 类型的指针
 * @retval  None
 */
void s_log_wave(int count, ...);

/**
 * @brief   输出 INFO 日志
 * @param   fmt 格式化字符串
 * @param   ... 参数
 * @retval  None
 */
void s_log_info(const char* fmt, ...);

/**
 * @brief   输出 WARN 日志
 * @param   fmt 格式化字符串
 * @param   ... 参数
 * @retval  None
 */
void s_log_warn(const char* fmt, ...);

/**
 * @brief   输出 ERROR 日志
 * @param   fmt 格式化字符串
 * @param   ... 参数
 * @retval  None
 */
void s_log_error(const char* fmt, ...);

#endif
