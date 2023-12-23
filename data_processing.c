// data_processing.c
#include <stddef.h>  // 或 #include <stdio.h> 或 #include <stdlib.h>
#include "data_processing.h"

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
