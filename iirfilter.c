#include "fdacoefs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#define TRUE 1
#define FALSE 0
int16_t int16_sat(float x);
struct HEADER {

    unsigned char riff[4];                      

    unsigned int overall_size   ;               
    
    unsigned char wave[4];                      

    unsigned char fmt_chunk_marker[4];          

    unsigned int length_of_fmt;                 

    unsigned int format_type;                   

    unsigned int channels;                     

    unsigned int sample_rate;                   
    unsigned int byterate;                     

    unsigned int block_align;                 

    unsigned int bits_per_sample;               
    unsigned char data_chunk_header [4];        

    unsigned int data_size;                    

};
FILE *ptr;
FILE *ptr_1;
struct HEADER header;
unsigned char buffer4[4];
unsigned char buffer2[2];
int read = 0;

int16_t int16_sat(float x)
{
	// 16 bits integer can hold maximum 32767 as decimal number. 
	// In this function if the floating point is bigger than this number
	// it is trancated to the maximum or minumum. Since the int16_t signed
	// integer. And the float x which is coming from f32_fir function gives
    if(x > 32767.0f)
        return 0x7FFF;

    if(x < -32768.0f)
        return -0x8000;

    return (int16_t)x;
} // Converting float to 2byte integer.

void iir_filter ( int16_t *input_signal,int num_samples, int16_t *channel_filter)
{
float shift_register[MWSPT_NSEC][3];
int m,n;
for(m=0; m< MWSPT_NSEC; m++){
	for(n=0;n<3;n++){
		shift_register[m][n] = 0.0;
	}
}

int i, j, k;
float temp,y,input;
for (k = 0; k < num_samples-1; k++) 
{	
	input = input_signal[k];
								
	for(i=0;i<MWSPT_NSEC;i++){		
		temp = (float)shift_register[i][1]/32768.0f; 
		shift_register[i][1] = (float)shift_register[i][0]/32768.0f;
		shift_register[i][2] = temp; 
		shift_register[i][0] = input - a[i][1]*shift_register[i][1] - a[i][2]*shift_register[i][2];
		y = b[i][0]*shift_register[i][0] + b[i][1]*shift_register[i][1] + b[i][0]*shift_register[i][2];	
		input = y;
	
	}

channel_filter [k] = int16_sat(input*32768);

}

}

int main(int argc, char **argv) {
	
	if(argc <2){
			printf("No wave file specified");
			return 0;
	}
	ptr = fopen(argv[1],"rb+");
	ptr_1 = fopen(argv[2],"wb+");
	// This function returns a FILE pointer
	if (ptr == NULL){
		printf("Error opening file");
		exit(1);
	}

	
	// START READING
	signed char temp[4];
	read = fread(header.riff,sizeof(header.riff),1,ptr);
	// 1-4 byte
	// write the riff
	fwrite(header.riff , 1 , sizeof(header.riff) ,ptr_1);
	read = fread(buffer4, sizeof(buffer4), 1, ptr);
	fwrite(buffer4, 1 , sizeof(buffer4) ,ptr_1);
	// 5-8 byte
	// convert little endian to big endian 4 byte int.
	header.overall_size  = buffer4[0] |
                        (buffer4[1]<<8) |
                        (buffer4[2]<<16) |
                        (buffer4[3]<<24);
    
 	read = fread(header.wave, sizeof(header.wave), 1, ptr);
 	fwrite(header.wave , 1 , sizeof(header.wave) ,ptr_1);
 	// 1 byte 9-12 since it is 4 bytes. 

	read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, ptr);
	fwrite(header.fmt_chunk_marker , 1 , sizeof(header.fmt_chunk_marker) ,ptr_1);
	// here the fmt_chunk_marker is 4 bytes, that is why fread will move the pointer 4 bytes more.
	
	read = fread(buffer4, sizeof(buffer4), 1, ptr);
	fwrite(buffer4 , 1 , sizeof(buffer4) ,ptr_1);

	// again we are using the same buffer for parsing 4 bytes data.
	
	header.length_of_fmt = buffer4[0] |
                            (buffer4[1] << 8) |
                            (buffer4[2] << 16) |
                            (buffer4[3] << 24);

	
	read = fread(buffer2, sizeof(buffer2), 1, ptr);
	fwrite(buffer2 , 1 , sizeof(buffer2) ,ptr_1);
	header.format_type = buffer2[0] | (buffer2[1] << 8);

char format_name[10] = "";
 if (header.format_type == 1)
   strcpy(format_name,"PCM");
 else if (header.format_type == 6)
  strcpy(format_name, "A-law");
 else if (header.format_type == 7)
  strcpy(format_name, "Mu-law");

 
read = fread(buffer2, sizeof(buffer2), 1, ptr);
fwrite(buffer2 , 1 , sizeof(buffer2) ,ptr_1);


header.channels = buffer2[0] | (buffer2[1] << 8);


read = fread(buffer4, sizeof(buffer4), 1, ptr);
fwrite(buffer4 , 1 , sizeof(buffer4) ,ptr_1);



header.sample_rate = buffer4[0] |
                        (buffer4[1] << 8) |
                        (buffer4[2] << 16) |
                        (buffer4[3] << 24);

read = fread(buffer4, sizeof(buffer4), 1, ptr);
fwrite(buffer4 , 1 , sizeof(buffer4) ,ptr_1);


header.byterate  = buffer4[0] |
                        (buffer4[1] << 8) |
                        (buffer4[2] << 16) |
                        (buffer4[3] << 24);


read = fread(buffer2, sizeof(buffer2), 1, ptr);
fwrite(buffer2 , 1 , sizeof(buffer2) ,ptr_1);

 
header.block_align = buffer2[0] |(buffer2[1] << 8);

read = fread(buffer2, sizeof(buffer2), 1, ptr);
fwrite(buffer2 , 1 , sizeof(buffer2) ,ptr_1);

header.bits_per_sample = buffer2[0] |
    (buffer2[1] << 8);

 read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, ptr);
 fwrite(header.data_chunk_header , 1 , sizeof(header.data_chunk_header) ,ptr_1);

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 fwrite(buffer4 , 1 , sizeof(buffer4) ,ptr_1);

 header.data_size = buffer4[0] |
                (buffer4[1] << 8) |
                (buffer4[2] << 16) |
                (buffer4[3] << 24 );


 // calculate the number of sample
 long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
 num_samples = num_samples / 2 ; 

long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
 // calculate duration of file
float duration_in_seconds = (float) header.overall_size / header.byterate;
long bytes_in_each_channel = (size_of_each_sample / header.channels);

long i =0;
int16_t channel_0;
int16_t channel_1;
int16_t channel_zero[num_samples];
int16_t channel_one[num_samples];
printf("SIZE OF EACH SAMPLE : %d\n",size_of_each_sample);
printf("BYTES IN EACH CHANNEL : %d\n",bytes_in_each_channel);
printf("NUMBER OF SAMPLES : %d\n",num_samples);
int byt_read;
 if (header.format_type == 1) {		
	for (i =1; i <= num_samples; i++) {  
		// Read data as  int16_t for channel_0
		read = fread(&channel_0, sizeof(channel_0), 1, ptr);
		channel_zero[i] = channel_0;
		// Read data as int16_t for channel_1	
		read = fread(&channel_1, sizeof(channel_1), 1, ptr);
		channel_one[i] = channel_1;
		// Each sample is 1 byte and misunderstanding is removed. 
		}
}
int16_t *channel_zero_filter;
int16_t *channel_one_filter;
channel_zero_filter = (int16_t*)malloc(num_samples*sizeof(int16_t));
channel_one_filter= (int16_t*)malloc(num_samples*sizeof(int16_t));	

iir_filter(channel_zero,num_samples,channel_zero_filter);
iir_filter(channel_one,num_samples,channel_one_filter);
int k;
for (k = 0; k < (num_samples-1); k++)  //  position in output
{
	//printf("AFTER FILTER : %d",channel_zero_filter[k]);
	fwrite(&channel_zero_filter[k],size_of_each_sample,1, ptr_1);
	fwrite(&channel_one_filter[k],size_of_each_sample,1, ptr_1);
}

// After writing the data, write the rest of the file.
 /*
 do{
 	byt_read = fread(buffer2,sizeof(buffer2),1,ptr);
 	fwrite(buffer2, 1 , sizeof(buffer2) ,ptr_1);     	
 }while(byt_read>0);
*/


fclose(ptr_1);
free(argv[2]);
fclose(ptr);
free(argv[1]);	
return 0;

}
