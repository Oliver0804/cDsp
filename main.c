
// main.c
#include <stdio.h>               // 為 printf 函數
#include <stdlib.h>
#include <string.h>  // Include this for strtok function

#include "helloworld.h"
#include "data_processing.h"     // 為 calculateMovingAverage 函數，假設它在這個頭文件中聲明


#include <stdio.h>
#include <stdlib.h>

int readCsvData(const char* filename, double* data, int maxDataSize, int targetColumn) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file!");
        return -1;  // 返回错误代码
    }

    char buffer[1024];
    int totalRows = 0;
    int index = 0;

    // 读取文件并提取目标列的数据
    while (fgets(buffer, sizeof(buffer), file)) {
        int currentColumn = 0;
        char* token = strtok(buffer, ",");
        while (token != NULL) {
            if (currentColumn == targetColumn && index < maxDataSize) {
                data[index++] = atof(token);
            }
            token = strtok(NULL, ",");
            currentColumn++;
        }
        totalRows++;
    }

    // 打印信息
    printf("Total Rows: %d\n", totalRows);
    printf("Data entries in target column (%d): %d\n", targetColumn, index);

    fclose(file);
    return index;  // 返回目标列的数据数量
}


int main() {
    printHelloWorld();

    const char* filename = "./demo.csv";  // CSV 文件名
    int maxDataSize = 100000;
    int windowSize = 13; // 窗口大小
    int startLine = 5;  // 开始的 targetLine
    int endLine = 10;   // 结束的 targetLine

    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (gnuplotPipe == NULL) {
        perror("Error opening pipe to gnuplot");
        return 1;
    }

    fprintf(gnuplotPipe, "set multiplot layout 2,3 title 'Data Visualization from Line %d to %d'\n", startLine, endLine);

    for (int targetLine = startLine; targetLine <= endLine; targetLine++) {
        double* inputData = malloc(sizeof(double) * maxDataSize);
        double* outputData = malloc(sizeof(double) * maxDataSize);

        if (inputData == NULL || outputData == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            pclose(gnuplotPipe);
            free(inputData);
            free(outputData);
            return 1;
        }

        int count = readCsvData(filename, inputData, maxDataSize, targetLine);
        if (count < 0) {
            fprintf(stderr, "Failed to read CSV data for line %d\n", targetLine);
            free(inputData);
            free(outputData);
            continue;
        }

        calculateMovingAverage(inputData, outputData, count, windowSize);

        // 将 inputData 和 outputData 写入一个临时文件
        char tempFileName[50];
        sprintf(tempFileName, "tempData_%d.temp", targetLine);
        FILE *tempFile = fopen(tempFileName, "w");
        for (int i = 0; i < count; i++) {
            fprintf(tempFile, "%d %f %f\n", i, inputData[i], outputData[i]);
        }
        fclose(tempFile);

        // 使用 gnuplot 绘制 inputData 和 outputData
        fprintf(gnuplotPipe, "set title 'Line %d'\n", targetLine);
        fprintf(gnuplotPipe, "plot '%s' using 1:2 with lines title 'Input', ", tempFileName);
        fprintf(gnuplotPipe, "'%s' using 1:3 with lines title 'Output'\n", tempFileName);

        free(inputData);
        free(outputData);
    }

    fprintf(gnuplotPipe, "unset multiplot\n");
    fflush(gnuplotPipe);
    pclose(gnuplotPipe);

    return 0;
}