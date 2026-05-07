/**
 * @file common.h
 * @brief 通用定义和数据结构
 * @details 定义全局常量、数据类型和通用工具函数
 * @author Radar Team
 * @date 2026-05-07
 */

#ifndef COMMON_H
#define COMMON_H

#include <complex>
#include <vector>
#include <cstdint>
#include <cmath>
#include <memory>

// ============================================================================
// 全局常量定义
// ============================================================================

/// LFM信号参数
constexpr int SAMPLES_PER_PULSE = 1668;      ///< 每个脉冲的采样点数
constexpr float PULSE_WIDTH = 500e-6f;       ///< 脉冲宽度 (500微秒)
constexpr float SAMPLE_RATE = SAMPLES_PER_PULSE / PULSE_WIDTH;  ///< 采样率 (Hz)
constexpr float CENTER_FREQUENCY = 10e9f;    ///< 中心频率 (10 GHz)
constexpr float BANDWIDTH = 100e6f;          ///< 带宽 (100 MHz)

/// MTD参数
constexpr int MTD_WINDOW_SIZE = 16;          ///< MTD滑动窗口大小
constexpr int MAX_PULSES_BUFFER = 1024;      ///< 最大脉冲缓冲数

/// CA-CFAR参数
constexpr int GUARD_CELLS = 2;               ///< 保护单元数
constexpr int TRAINING_CELLS = 4;            ///< 训练单元数
constexpr float CFAR_PFA = 1e-5f;            ///< 虚警概率

/// 多线程参数
constexpr int MAX_THREADS = 16;              ///< 最大线程数
constexpr int DATA_QUEUE_SIZE = 128;         ///< 数据队列大小

/// 运行测试参数
constexpr int TEST_PULSES_COUNT = 100;       ///< 测试脉冲数
constexpr int LONG_RUN_PULSES = 10000;       ///< 长时间运行脉冲数

// ============================================================================
// 数据类型定义
// ============================================================================

/// 复数类型（单精度浮点）
using ComplexFloat = std::complex<float>;

/// 复数向量类型
using ComplexVector = std::vector<ComplexFloat>;

/// 浮点向量类型
using FloatVector = std::vector<float>;

/// 整数向量类型
using IntVector = std::vector<int>;

// ============================================================================
// 信号处理数据结构
// ============================================================================

/**
 * @struct LFMSignal
 * @brief LFM信号数据结构
 */
struct LFMSignal {
    int pulse_id;                    ///< 脉冲编号
    ComplexVector samples;           ///< 复数采样数据
    float timestamp;                 ///< 时间戳 (秒)
    
    LFMSignal() : pulse_id(-1), timestamp(0.0f) {
        samples.resize(SAMPLES_PER_PULSE);
    }
    
    explicit LFMSignal(int id) : pulse_id(id), timestamp(0.0f) {
        samples.resize(SAMPLES_PER_PULSE);
    }
};

/**
 * @struct PulseCompressionOutput
 * @brief 脉冲压缩输出数据结构
 */
struct PulseCompressionOutput {
    int pulse_id;                    ///< 脉冲编号
    FloatVector magnitude;           ///< 幅度响应
    FloatVector phase;               ///< 相位响应
    float peak_power;                ///< 峰值功率
    int peak_index;                  ///< 峰值位置
    
    PulseCompressionOutput() : pulse_id(-1), peak_power(0.0f), peak_index(-1) {
        magnitude.resize(SAMPLES_PER_PULSE);
        phase.resize(SAMPLES_PER_PULSE);
    }
};

/**
 * @struct MTDOutput
 * @brief MTD处理输出数据结构
 */
struct MTDOutput {
    int pulse_id;                    ///< 脉冲编号
    FloatVector doppler_response;    ///< 多普勒响应(MTD窗口后)
    int doppler_bin;                 ///< 多普勒单元号
    float doppler_power;             ///< 多普勒功率
    
    MTDOutput() : pulse_id(-1), doppler_bin(-1), doppler_power(0.0f) {
        doppler_response.resize(MTD_WINDOW_SIZE);
    }
};

/**
 * @struct CFARDetectionResult
 * @brief CA-CFAR检测结果数据结构
 */
struct CFARDetectionResult {
    int pulse_id;                    ///< 脉冲编号
    int channel_id;                  ///< 通道编号
    float threshold;                 ///< 检测阈值
    float peak_power;                ///< 目标功率
    int target_range_bin;            ///< 距离单元
    int target_doppler_bin;          ///< 多普勒单元
    bool is_detected;                ///< 是否检测到目标
    
    CFARDetectionResult() : pulse_id(-1), channel_id(-1), 
                           threshold(0.0f), peak_power(0.0f),
                           target_range_bin(-1), target_doppler_bin(-1),
                           is_detected(false) {}
};

/**
 * @struct RadarOutput
 * @brief 最终雷达输出结果
 */
struct RadarOutput {
    int pulse_id;                    ///< 脉冲编号
    int channel_id;                  ///< 获胜通道编号
    float peak_power;                ///< 峰值功率
    int range_bin;                   ///< 距离单元
    int doppler_bin;                 ///< 多普勒单元
    float doppler_velocity;          ///< 多普勒速度 (m/s)
    float range_m;                   ///< 距离 (米)
    uint64_t timestamp_us;           ///< 时间戳 (微秒)
    
    RadarOutput() : pulse_id(-1), channel_id(-1), peak_power(0.0f),
                   range_bin(-1), doppler_bin(-1), doppler_velocity(0.0f),
                   range_m(0.0f), timestamp_us(0) {}
};

// ============================================================================
// 实用函数
// ============================================================================

/**
 * @brief 计算汉宁窗系数
 * @param n 窗口大小
 * @return 窗口系数向量
 */
inline FloatVector getHanningWindow(int n) {
    FloatVector window(n);
    for (int i = 0; i < n; ++i) {
        window[i] = 0.5f * (1.0f - cosf(2.0f * M_PI * i / (n - 1)));
    }
    return window;
}

/**
 * @brief 计算功率谱
 * @param signal 复数信号
 * @return 功率谱向量
 */
inline FloatVector getPowerSpectrum(const ComplexVector& signal) {
    FloatVector spectrum(signal.size());
    for (size_t i = 0; i < signal.size(); ++i) {
        float real = signal[i].real();
        float imag = signal[i].imag();
        spectrum[i] = real * real + imag * imag;
    }
    return spectrum;
}

/**
 * @brief 归一化向量
 * @param vec 输入向量
 * @return 归一化后的向量
 */
inline FloatVector normalize(const FloatVector& vec) {
    FloatVector result = vec;
    float max_val = 0.0f;
    
    for (float v : vec) {
        if (v > max_val) max_val = v;
    }
    
    if (max_val > 0.0f) {
        for (float& v : result) {
            v /= max_val;
        }
    }
    
    return result;
}

/**
 * @brief 查找向量最大值索引
 * @param vec 输入向量
 * @return 最大值的索引
 */
inline int findMaxIndex(const FloatVector& vec) {
    int max_idx = 0;
    float max_val = vec[0];
    
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i] > max_val) {
            max_val = vec[i];
            max_idx = i;
        }
    }
    
    return max_idx;
}

#endif // COMMON_H
