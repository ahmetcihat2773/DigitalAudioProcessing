#include "fdacoeffs.h"
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
void fir_filter (int8_t *input_signal,int num_samples,int8_t *channel_filter);
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


long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
 // calculate duration of file
float duration_in_seconds = (float) header.overall_size / header.byterate;
long bytes_in_each_channel = (size_of_each_sample / header.channels);

long i =0;
int16_t channel_0;
int16_t channel_1;
printf("SIZE OF EACH SAMPLE : %d\n",size_of_each_sample);
printf("BYTES IN EACH CHANNEL : %d\n",bytes_in_each_channel);
printf("NUMBER OF SAMPLES : %d\n",num_samples);
// Each left channel and right channel samples instance is 2bytes.
 if (header.format_type == 1) {		
	for (i =1; i <= num_samples; i++) {  
		// Read data as  int16_t for channel_0
		read = fread(&channel_0, sizeof(channel_0), 1, ptr);
		// Read data as int16_t for channel_1	
		read = fread(&channel_1, sizeof(channel_1), 1, ptr);
		fwrite(&channel_0,size_of_each_sample,1,ptr_1);
		fwrite(&channel_1,size_of_each_sample,1,ptr_1);
		// What I see is each sample is 1byte I dont know why
		}
}
	
	fclose(ptr_1);
 	free(argv[2]);
 	fclose(ptr);
	free(argv[1]);	
	return 0;

}
