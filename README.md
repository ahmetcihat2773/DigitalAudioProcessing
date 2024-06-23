# Audio Processing Project

## Overview

The primary focus of this porject is on filtering techniques, including FIR (Finite Impulse Response) and IIR (Infinite Impulse Response) filters, as well as reading and writing audio data.

## Contents

1. [checkChannels.py](#1-checkchannelspy)
2. [filter.c](#2-filterc)
3. [firFilter.c](#3-firfilterc)
4. [iirfilter.c](#4-iirfilterc)
5. [ReadWrite.c](#5-readwritec)

## 1. checkChannels.py

### Purpose
The `checkChannels.py` script is used to analyze and visualize audio channels from an audio file. It likely reads the audio data, processes it to separate different channels, and then visualizes them for analysis.

### Techniques Used
- **Audio Data Reading:** The script reads audio data from a file.
- **Channel Separation:** It separates the audio data into different channels (e.g., left and right for stereo audio).
- **Visualization:** Uses libraries like Matplotlib to plot and visualize the audio signals from different channels.

## 2. filter.c

### Purpose
The `filter.c` file likely contains implementations for basic audio filtering operations. It might include functions for applying various types of filters to an audio signal.

### Techniques Used
- **Filtering Algorithms:** Implementation of basic filtering algorithms such as low-pass, high-pass, band-pass, and band-stop filters.
- **Signal Processing:** Techniques to manipulate and filter audio signals in the time domain.

## 3. firFilter.c

### Purpose
The `firFilter.c` file implements FIR (Finite Impulse Response) filters for audio signal processing. FIR filters are a type of digital filter that relies on a finite number of previous input values.

### Techniques Used
- **FIR Filter Design:** Design of FIR filters using various windowing techniques.
- **Convolution:** Applying the filter to the audio signal using convolution.
- **Impulse Response:** Utilizes a finite impulse response to determine the filter output.

## 4. iirfilter.c

### Purpose
The `iirfilter.c` file implements IIR (Infinite Impulse Response) filters for audio signal processing. IIR filters use feedback and rely on both previous input and output values.

### Techniques Used
- **IIR Filter Design:** Design of IIR filters using techniques like Butterworth, Chebyshev, and Elliptic filter design.
- **Recursive Filtering:** Uses recursive algorithms to apply the filter to the audio signal.
- **Stability and Phase Response:** Ensuring filter stability and managing phase response through feedback mechanisms.

## 5. ReadWrite.c

### Purpose
The `ReadWrite.c` file is responsible for reading and writing audio data to and from files. It handles the input and output operations necessary for processing audio signals.

### Techniques Used
- **File I/O:** Reading from and writing to audio files in various formats (e.g., WAV, MP3).
- **Buffer Management:** Managing buffers to efficiently handle large audio data.
- **Audio Format Conversion:** Converting between different audio formats and bit depths.

## Getting Started

To get started with this project, you can compile and run the C files using a C compiler. For example:
```sh
gcc -o filter filter.c
./filter
