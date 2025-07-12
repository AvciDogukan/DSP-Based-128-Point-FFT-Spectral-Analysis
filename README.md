
# TMS320F28335 Real-Time FFT Analyzer

This project demonstrates a **real-time 128-point Fast Fourier Transform (FFT)** implementation on the Texas Instruments TMS320F28335 DSP. It processes time-domain signals from the ADC, transforms them into the frequency domain, measures the FFT execution time, and automatically identifies the two most dominant frequency components in the input signal.

---

## 🚀 Features

- **Fully functional, C-based 128-point FFT algorithm** (`FFT128.c`)
- Designed for real-time execution on the **TI TMS320F28335 DSP**
- **ADC signal acquisition** (from buffer), conversion to complex type
- **Precomputed twiddle factors** for efficient FFT calculation
- **High-precision execution time measurement** using hardware timers (1 μs resolution)
- **Magnitude spectrum computation** after FFT
- **Automatic detection of the two strongest frequencies**
- **Configurable sampling frequency** (default: 8 kHz)
- Clean, commented code for education and research

---

## 📂 File Structure

project/
│
├── main.c          # Main program for signal processing, FFT, timing and frequency detection
├── FFT128.c        # C-callable FFT algorithm implementation (128-point, radix-2)
├── x_buffer_6.h    # Input signal buffer (external, not provided)
├── README.md       # Project documentation

---

## 🛠️ How It Works

1. **Initializes** the TMS320F28335 system clocks, disables the watchdog, and sets up high-resolution timers.
2. **Precalculates FFT twiddle factors** (roots of unity).
3. **Reads ADC data** from an input buffer and normalizes it.
4. **Performs the 128-point FFT** using a custom C implementation.
5. **Calculates the magnitude spectrum** (abs(FFT result)).
6. **Finds the two largest frequency peaks** in the spectrum (excluding DC).
7. **Converts peak bin indices to Hz** using the sampling frequency.
8. **Measures and reports execution time** in microseconds.

---

## 💻 Example Code Snippet

// --- Main FFT computation ---
FFT128(samples, PTS);

// --- Magnitude calculation ---
for (i = 0; i < PTS; i++) {
    x1[i] = sqrt(samples[i].real * samples[i].real + samples[i].imag * samples[i].imag);
}

// --- Peak frequency detection ---
float fs = 8000.0f; // Sampling frequency (Hz)
float frq1 = (fs * max1_index) / PTS; // First dominant frequency (Hz)
float frq2 = (fs * max2_index) / PTS; // Second dominant frequency (Hz)

---

## ⚡️ Requirements

- **Target:** TI TMS320F28335 DSP (ControlCARD or compatible board)
- **Compiler:** Code Composer Studio or compatible C compiler for TI DSPs
- math.h library for sqrt, sin, cos
- x_buffer_6.h (must provide input buffer: extern float xn[128];)

---

## 📊 Applications

- DSP lab courses & education
- Embedded system spectral analysis
- Real-time signal processing demonstrations
- Frequency analysis & peak detection in hardware

---

## 📝 License

MIT License

---

## 👨‍💻 Author

Developed by **Doğukan AVCI** 
Date: 10.05.2025

- Email: hulavci121@gmail.com
- GitHub: https://github.com/AvciDogukan
- LinkedIn: https://www.linkedin.com/in/doğukanavcı-119541229/

For questions or collaboration, feel free to get in touch!
