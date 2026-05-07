/**
 * @file config.h
 * @brief 系统配置参数定义
 * @details 定义LFM雷达信号处理系统的所有关键参数
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <cstddef>
#include <cstdint>

// ==================== 信号参数 ====================
/** @brief 每个脉冲的采样点数 */
constexpr size_t SAMPLES_PER_PULSE = 1668;

/** @brief 脉冲宽度（微秒） */
constexpr float PULSE_WIDTH_US = 500.0f;

/** @brief 采样率（Hz） */
constexpr float SAMPLE_RATE = 1e6;  // 1 MHz

/** @brief 中心频率（Hz） */
constexpr float CENTER_FREQ = 10e9;  // 10 GHz

/** @brief LFM扫频宽度（Hz） */
constexpr float CHIRP_BANDWIDTH = 100e6;  // 100 MHz

// ==================== MTD处理参数 ====================
/** @brief MTD滑动窗口大小 */
constexpr size_t MTD_WINDOW_SIZE = 16;

/** @brief MTD处理的最大脉冲缓冲数 */
constexpr size_t MTD_MAX_PULSES = 32;

// ==================== CA-CFAR参数 ====================
/** @brief CFAR保护单元数 */
constexpr size_t CFAR_GUARD_CELLS = 4;

/** @brief CFAR参考单元数（单侧） */
constexpr size_t CFAR_REFERENCE_CELLS = 16;

/** @brief CFAR虚警概率（Pfa） */
constexpr float CFAR_PFA = 1e-4f;

// ==================== 多线程参数 ====================
/** @brief 系统最大线程数 */
constexpr uint32_t MAX_THREADS = 16;

/** @brief 数据造数线程数 */
constexpr uint32_t DATA_GEN_THREADS = 2;

/** @brief 信号处理线程数 */
constexpr uint32_t PROCESSING_THREADS = 12;

/** @brief 结果输出线程数 */
constexpr uint32_t OUTPUT_THREADS = 1;

// ==================== 缓冲区参数 ====================
/** @brief 环形缓冲区大小（脉冲数） */
constexpr size_t RING_BUFFER_SIZE = 128;

/** @brief 输出缓冲区大小（检测结果数） */
constexpr size_t OUTPUT_BUFFER_SIZE = 256;

// ==================== 运行参数 ====================
/** @brief 总处理脉冲数（0表示持续运行） */
constexpr uint64_t TOTAL_PULSES = 0;  // 无限循环

/** @brief 运行时长（秒，0表示无限） */
constexpr uint32_t RUN_DURATION_SEC = 0;

/** @brief 是否启用性能统计 */
constexpr bool ENABLE_STATS = true;

/** @brief 统计输出间隔（脉冲数） */
constexpr size_t STATS_INTERVAL = 100;

// ==================== 仿真参数 ====================
/** @brief 是否使用仿真信号 */
constexpr bool USE_SIMULATED_DATA = true;

/** @brief 仿真目标数量 */
constexpr size_t NUM_SIMULATED_TARGETS = 3;

/** @brief 仿真目标信噪比（dB） */
constexpr float SIMULATED_SNR_DB = 15.0f;

// ==================== 调试参数 ====================
/** @brief 是否启用详细日志 */
constexpr bool ENABLE_VERBOSE_LOG = false;

/** @brief 是否输出中间处理结果 */
constexpr bool DUMP_INTERMEDIATE_RESULTS = false;

/** @brief 中间结果输出目录 */
constexpr const char* DEBUG_OUTPUT_DIR = "./debug_output/";

#endif  // CONFIG_H
