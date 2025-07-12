/***************************************************************
 * TMS320F28335 DSP – Real-Time FFT Execution and Analysis
 *
 * This program demonstrates a complete implementation of a
 * 128-point Fast Fourier Transform (FFT) using the TI
 * TMS320F28335 DSP. The goal is to convert a time-domain
 * real-valued signal into its frequency-domain representation,
 * identify the two most dominant frequency components, and
 * measure the FFT execution time with microsecond precision.
 *
 * Key Features:
 * --------------
 * - Initialization of system clock, watchdog timer, and CPU timers
 * - Pre-calculation of twiddle (rotation) factors for FFT
 * - Conversion of real-valued ADC input to complex number format
 * - Execution of external FFT routine
 * - Real-time timing using CpuTimer0 for performance benchmarking
 * - Computation of magnitude spectrum from FFT result
 * - Peak detection to find the two strongest frequency components
 * - Frequency resolution based on known sampling rate
 *
 * This code is designed to be run on the TI DSP development
 * board (F28335 ControlCARD). It provides a practical example
 * of how digital signal processing operations—such as spectral
 * analysis—can be executed efficiently in embedded systems with
 * real-time constraints.
 *
 * Author: Doğukan AVCI, Sinan İLBEY, Tunahan ARIKANLI
 * Date: 10.05.2025
 * Target: Texas Instruments TMS320F28335 DSP
 ***************************************************************/


#include "DSP2833x_Device.h"           // Includes device-specific definitions (registers, etc.)
#include <math.h>                      // Math library for trigonometric functions
#include "DSP2833x_Examples.h"         // Includes example functions for initialization
#include "x_buffer_6.h"                // Input signal buffer (extern float xn[128])

// Global constants
#define PTS 128                        // Number of FFT points
#define PI 3.14159265358979            // Definition of π

// Define a complex number structure
typedef struct {
    float real;                        // Real part of the complex number
    float imag;                        // Imaginary part of the complex number
} COMPLEX;

// External FFT function (implemented elsewhere, e.g., assembly or separate C file)
extern void FFT128(COMPLEX *Y, int n);  // Computes 128-point FFT on array Y

// Global variables
COMPLEX w[PTS];                        // Twiddle factors (complex exponentials)
COMPLEX samples[PTS];                 // Input signal samples (complex form)
float x1[PTS];                        // Magnitude spectrum array
short i;                              // Loop counter

void main(void)
{
    // Initialize system clock, PLL, peripheral clocks, etc.
    InitSysCtrl();

    // Disable watchdog timer (prevents unwanted system resets)
    EALLOW;
    SysCtrlRegs.WDCR = 0x00AF;
    EDIS;

    // Disable all CPU interrupts during configuration
    DINT;

    // Initialize CPU timers (Timer0, Timer1, Timer2)
    InitCpuTimers();

    // Configure Timer0: CPU clock = 150 MHz, resolution = 1 microsecond
    ConfigCpuTimer(&CpuTimer0, 150, 1);

    // Precompute FFT twiddle factors (roots of unity)
    for (i = 0; i < PTS; i++) {
        w[i].real = cos(2 * PI * i / PTS);      // Real part: cos(2πi/N)
        w[i].imag = -sin(2 * PI * i / PTS);     // Imaginary part: -sin(2πi/N)
    }

    // Normalize input signal from buffer and set imaginary part to 0
    for (i = 0; i < PTS; i++) {
        samples[i].real = xn[i] / 32768.0f;     // Scale from int16 to float in range [-1, 1]
        samples[i].imag = 0.0f;                 // ADC data is real-valued; imaginary part = 0
    }

    // Start Timer0 (TSS = 0 means timer is enabled)
    CpuTimer0Regs.TCR.bit.TSS = 0;
    Uint32 start_time = CpuTimer0Regs.TIM.all; // Capture current timer value (in microseconds)

    // Perform FFT computation
    FFT128(samples, PTS);

    // Stop Timer0 after FFT completes
    Uint32 end_time = CpuTimer0Regs.TIM.all;   // Capture end time
    CpuTimer0Regs.TCR.bit.TSS = 1;             // Stop the timer (TSS = 1)

    // Calculate elapsed time in clock cycles (Timer counts down)
    Uint32 elapsed_clocks = start_time - end_time;
    float elapsed_us = (float)elapsed_clocks;  // Since resolution is 1 μs, value is already in μs

    // Compute magnitude spectrum: sqrt(real² + imag²)
    for (i = 0; i < PTS; i++) {
        x1[i] = sqrt(samples[i].real * samples[i].real + samples[i].imag * samples[i].imag);
    }

    // Find the two most dominant frequency components (largest magnitudes)
    int max1_index = 0, max2_index = 0;
    float max1 = 0.0f, max2 = 0.0f;

    // Search only the first half of the spectrum (Nyquist limit)
    for (i = 1; i < PTS / 2; i++) {
        if (x1[i] > max1) {
            max2 = max1;               // Shift previous max to second place
            max2_index = max1_index;
            max1 = x1[i];              // Update new maximum
            max1_index = i;
        } else if (x1[i] > max2) {
            max2 = x1[i];              // Update second maximum if larger than previous
            max2_index = i;
        }
    }

    // Calculate actual frequency values from bin indices
    float fs = 8000.0f;                // Sampling frequency in Hz (assumed)
    float frq1 = (fs * max1_index) / PTS; // Frequency of first dominant peak
    float frq2 = (fs * max2_index) / PTS; // Frequency of second dominant peak

    // Dummy variable to prevent compiler optimizations and allow watch in CCS debugger

    volatile float dummy = frq1 + frq2 + elapsed_us;


    while (1);                         // Infinite loop to prevent program exit
}

