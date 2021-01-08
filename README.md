# changeChannels
This program changes the two channels in storeo wav file into one another. You can run the program with the following commands

    gcc -o filter filter.c 
    
    filter input.wav output.wav

Here the output.wav is the output file in which channels are changed and the input file is the source wav file. 

Normally signals should be the same since the two signal is taken from right channels of the wav files however I have changed them that's why there are some voltage differences which can be seen in the figure below.
![Rigth channel of input"ORANGE" Right channel of output"Red"](https://github.com/ahmetcihat2773/changeChannels/blob/main/Capture.PNG)
