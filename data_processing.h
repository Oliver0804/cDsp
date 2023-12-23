// data_processing.h

#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H
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
void calculateMovingAverage(const double* inputData, double* outputData, int dataSize, int windowSize);



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
void butterworthLowPassFilter(const double* inputData, double* outputData, int dataSize, double cutoffFrequency, double samplingRate);

/**
 * Detects movement based on the second derivative (acceleration) of the input data.
 *
 * This function computes the second derivative of the input data, which represents position,
 * to detect significant movements. Movement is identified by finding where the acceleration
 * (the second derivative of position) exceeds a specified threshold. This method is useful
 * in scenarios where movement can be inferred from changes in acceleration.
 *
 * @param inputData Pointer to the array of input data representing position.
 *                  The data should be evenly sampled and in a consistent unit.
 * @param outputData Pointer to the array where the movement detection results will be stored.
 *                   After processing, this array will contain values of 1.0 for indices
 *                   where movement is detected and 0.0 otherwise.
 * @param dataSize The size of the inputData and outputData arrays, indicating the
 *                 number of elements they contain.
 * @param threshold The threshold for acceleration above which movement is considered to be detected.
 *                  This value should be chosen based on the expected level of movement and
 *                  the characteristics of the input data.
 * @param samplingRate The sampling rate of the input data in samples per second (Hz).
 *                     This is used to calculate the time interval between samples.
 *
 * @note The function assumes that the input data is uniformly sampled. It calculates
 *       the acceleration using a simple finite difference method and then compares it
 *       against the threshold. The first and last elements of the outputData array are
 *       always set to 0.0 as the second derivative cannot be computed at these points.
 *
 * Example usage:
 *     double positionData[100]; // Some position data
 *     double movementDetection[100];
 *     int dataSize = 100;
 *     double threshold = 0.5; // Set based on expected movement level
 *     double samplingRate = 50.0; // in Hz
 *     detectMovement(positionData, movementDetection, dataSize, threshold, samplingRate);
 */
void detectMovement(const double* inputData, double* outputData, int dataSize, double threshold, double samplingRate);
/**
 * Applies Zero Velocity Update (ZUPT) to the given velocity data based on acceleration data.
 *
 * This function uses acceleration data to identify periods of zero velocity and applies
 * a zero velocity update to the velocity data accordingly. It checks if the acceleration
 * remains below a specified threshold over a consecutive number of data points, indicating
 * a stationary state.
 *
 * @param velocityData Pointer to the array of velocity data that will be updated.
 *                     If zero velocity is detected, corresponding velocity values will be set to zero.
 * @param accelData Pointer to the array of acceleration data used for zero velocity detection.
 * @param dataSize The size of the velocityData and accelData arrays.
 * @param threshold The threshold for acceleration below which the object is considered stationary.
 *                  If the acceleration magnitude is less than this threshold over a consecutive
 *                  number of points, it's assumed that the velocity should be zero.
 * @param continuousCountThreshold The minimum number of consecutive data points below the acceleration
 *                                 threshold required to consider the object stationary and apply ZUPT.
 *
 * @return Returns 1 if zero velocity is detected, or 0 if no zero velocity is detected.
 *         Returns -1 in case of an error (e.g., null pointers or invalid dataSize).
 *
 * @note This function assumes uniform sampling of the input data and does not handle cases
 *       where acceleration data may be noisy or sporadic. Further filtering may be required
 *       in such cases. The function also assumes that the velocity data is updated elsewhere
 *       and only sets it to zero when a stationary state is detected.
 */
int applyZupt(double* velocityData, const double* accelData, int dataSize, double threshold, int continuousCountThreshold);

void analyzeWalking(const double* inputData, int dataSize, int* stepCount, double* avgStepDistance);





#endif // DATA_PROCESSING_H
