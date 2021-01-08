import matplotlib
import numpy as np
from scipy.io import wavfile
import matplotlib.pyplot as plt
input_samplerate, input_data = wavfile.read('input.wav')
output_samplerate, output_data = wavfile.read('output.wav')

input_channelLeft = input_data[:, 0]
input_channelRight = input_data[:, 1]

output_channelLeft = output_data[:, 0]
output_channelRight = output_data[:, 1]

plt.figure()
plt.plot(input_channelLeft)
plt.plot(output_channelLeft)
plt.show()
#Input left channel should be the same output right channel.