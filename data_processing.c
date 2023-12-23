// data_processing.c
#include <stddef.h>  // 或 #include <stdio.h> 或 #include <stdlib.h>
#include <math.h>

#include "data_processing.h"
/**
 * Calculates the moving average of the given input data.
 *
 * This function computes the moving average of the input data array. The moving average
 * is calculated over a specified window size, which determines how many elements are
 * considered for each average calculation. This technique is commonly used for smoothing
 * out short-term fluctuations and highlighting longer-term trends or cycles.
 *
 * @param inputData Pointer to the array of input data. This array contains the data
 *                  over which the moving average is to be computed.
 * @param outputData Pointer to the array where the calculated moving average values
 *                   will be stored. This array should be pre-allocated and have the
 *                   same size as inputData.
 * @param dataSize The size of the inputData and outputData arrays, indicating the
 *                 number of elements they contain.
 * @param windowSize The size of the moving window. This parameter specifies the
 *                   number of elements to be considered for each average calculation.
 *                   It must be less than or equal to dataSize.
 *
 * @note If inputData or outputData is NULL, or if dataSize or windowSize is non-positive,
 *       the function will return early without performing any computations.
 *       The function handles boundary cases by averaging over the available elements
 *       at the start of the input array, which means the first few averages are based on
 *       fewer elements than the specified windowSize.
 *
 * Example usage:
 *     double data[] = {1.0, 2.0, 3.0, 4.0, 5.0};
 *     double avgData[5];
 *     int dataSize = 5;
 *     int windowSize = 3;
 *     calculateMovingAverage(data, avgData, dataSize, windowSize);
 */
void calculateMovingAverage(const double* inputData, double* outputData, int dataSize, int windowSize) {
    if (inputData == NULL || outputData == NULL || dataSize <= 0 || windowSize <= 0) {
        return; // 錯誤處理：空指針或無效大小
    }

    for (int i = 0; i < dataSize; ++i) {
        double sum = 0.0;
        int count = 0;
        for (int j = i; j >= 0 && count < windowSize; --j, ++count) {
            sum += inputData[j];
        }
        outputData[i] = sum / count;
    }
}

/**
 * Implements a simple first-order low-pass Butterworth filter.
 *
 * This function applies a low-pass Butterworth filter to the input data. The Butterworth filter
 * is chosen for its smooth frequency response in the passband. It attenuates the components
 * of the signal that have frequencies higher than the specified cutoff frequency.
 *
 * @param inputData Pointer to the array of input data to be filtered.
 *                  This should be an array of double-precision floating-point numbers.
 * @param outputData Pointer to the array where the filtered output data will be stored.
 *                   This array should be pre-allocated and have the same size as inputData.
 * @param dataSize The number of elements in the inputData and outputData arrays.
 * @param cutoffFrequency The cutoff frequency for the low-pass filter in Hertz (Hz).
 *                        Frequencies above this threshold will be attenuated.
 * @param samplingRate The sampling rate of the input data in Hertz (Hz).
 *                     It is used to calculate the time constant of the filter.
 *
 * @note The filter is a first-order filter, providing a 6 dB/octave slope beyond the cutoff.
 *       This implementation assumes uniform sampling of the input signal and does not
 *       handle initialization conditions, which might be necessary for some applications.
 *       For more complex requirements, a higher-order filter or a different filter design
 *       might be needed.
 *
 * Example usage:
 *     double data[100]; // Some input data
 *     double filteredData[100];
 *     int dataSize = 100;
 *     double cutoffFrequency = 5.0; // in Hz
 *     double samplingRate = 50.0; // in Hz
 *     butterworthLowPassFilter(data, filteredData, dataSize, cutoffFrequency, samplingRate);
 */

void butterworthLowPassFilter(const double* inputData, double* outputData, int dataSize, double cutoffFrequency, double samplingRate) {
    if (inputData == NULL || outputData == NULL || dataSize <= 0 || cutoffFrequency <= 0 || samplingRate <= 0) {
        return; 
    }

    double dt = 1.0 / samplingRate;
    double RC = 1.0 / (2 * 3.14159265358979323846 * cutoffFrequency);
    double alpha = dt / (RC + dt);

    outputData[0] = inputData[0];
    for (int i = 1; i < dataSize; ++i) {
        outputData[i] = alpha * inputData[i] + (1 - alpha) * outputData[i - 1];
    }
}




void detectMovement(const double* inputData, double* outputData, int dataSize, double threshold, double samplingRate) {
    if (inputData == NULL || outputData == NULL || dataSize < 3 || samplingRate <= 0) {
        return; // 错误处理：空指针、无效大小或采样率
    }

    double dt = 1.0 / samplingRate;
    double dtSquared = dt * dt;

    // 初始化输出数组
    for (int i = 0; i < dataSize; ++i) {
        outputData[i] = 0.0;
    }

    // 计算二阶微分（加速度）
    for (int i = 1; i < dataSize - 1; ++i) {
        double acceleration = (inputData[i+1] - 2 * inputData[i] + inputData[i-1]) / dtSquared;
        outputData[i] = fabs(acceleration); // 存储绝对值加速度
    }

    // 标记超过阈值的加速度为运动检测
    for (int i = 0; i < dataSize; ++i) {
        if (outputData[i] > threshold) {
            outputData[i] = 1.0; // 表示检测到运动
        } else {
            outputData[i] = 0.0; // 表示未检测到运动
        }
    }
}



int applyZupt(double* velocityData, const double* accelData, int dataSize, double threshold, int continuousCountThreshold) {
    if (velocityData == NULL || accelData == NULL || dataSize <= 0) {
        return -1; // 错误处理：空指针或无效大小
    }

    int continuousCount = 0;
    for (int i = 0; i < dataSize; ++i) {
        if (fabs(accelData[i]) < threshold) {
            continuousCount++;
            velocityData[i] = 0.0;
            if (continuousCount >= continuousCountThreshold) {
                return 1; // 检测到零速度
            }
        } else {
            continuousCount = 0; // 重置连续计数
            // ... 对速度的正常更新 ...
        }
    }
    return 0; // 未检测到零速度
}

//1-Aixs
void applyLowPassFilter(double* data, int dataSize) {
    const double alpha = 0.1; // 这个值决定了滤波器的强度，可能需要调整
    double filteredValue = data[0];

    for (int i = 1; i < dataSize; ++i) {
        filteredValue = alpha * data[i] + (1 - alpha) * filteredValue;
        data[i] = filteredValue;
    }
}

// bias
void subtractBias(double* data, int dataSize, double bias) {
    for (int i = 0; i < dataSize; ++i) {
        data[i] -= bias;
    }
}
