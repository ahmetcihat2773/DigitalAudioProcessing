#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// WAVE file header format
struct HEADER {

    unsigned char riff[4];                      // RIFF string

    unsigned int overall_size   ;               // overall size of file in bytes

    unsigned char wave[4];                      // WAVE string

    unsigned char fmt_chunk_marker[4];          // fmt string with trailing null char

    unsigned int length_of_fmt;                 // length of the format data

    unsigned int format_type;                   // format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law

    unsigned int channels;                      // no.of channels

    unsigned int sample_rate;                   // sampling rate (blocks per second)

    unsigned int byterate;                      // SampleRate * NumChannels * BitsPerSample/8

    unsigned int block_align;                   // NumChannels * BitsPerSample/8

    unsigned int bits_per_sample;               // bits per sample, 8- 8bits, 16- 16 bits etc
    unsigned char data_chunk_header [4];        // DATA string or FLLR string

    unsigned int data_size;                     // NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read

};

FILE *ptr;
char *filename;
struct HEADER header;
unsigned char buffer4[4];
unsigned char buffer2[2];
char* seconds_to_time(float seconds);

int main(int argc, char **argv) {
	
	if(argc <2){
			printf("No wave file specified");
			return 0;
	}
	//printf("%s",argv[1]);
	// OPEN FILE
	ptr = fopen(argv[1],"rb");
	if (ptr == NULL){
		printf("Error opening file");
		exit(1);
	}
	// START READING
	int read = 0;
	read = fread(header.riff,sizeof(header.riff),1,ptr);
	printf("%s\n",header.riff);
	
	// %u is an unsigned integer.
	read = fread(buffer4, sizeof(buffer4), 1, ptr);
 	printf("Chunk Size : (Liddle Endian):\t%u\t%u\t%u\t%u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
	// convert little endian to big endian 4 byte int.
	header.overall_size  = buffer4[0] |
                        (buffer4[1]<<8) |
                        (buffer4[2]<<16) |
                        (buffer4[3]<<24);
    printf("(5-8) Overall size: bytes:%u, Kb:%u \n", header.overall_size, header.overall_size/1024);

	
 	read = fread(header.wave, sizeof(header.wave), 1, ptr);
 	printf("(9-12) Wave marker: %s\n", header.wave);

	read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, ptr);
	printf("(13-16) Fmt marker: %s\n", header.fmt_chunk_marker);
	
	read = fread(buffer4, sizeof(buffer4), 1, ptr);
	printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
	
	header.length_of_fmt = buffer4[0] |
                            (buffer4[1] << 8) |
                            (buffer4[2] << 16) |
                            (buffer4[3] << 24);
 	printf("(17-20) Length of Fmt header: %u \n", header.length_of_fmt);
	
	read = fread(buffer2, sizeof(buffer2), 1, ptr); printf("%u %u \n", buffer2[0], buffer2[1]);
	header.format_type = buffer2[0] | (buffer2[1] << 8);

char format_name[10] = "";
 if (header.format_type == 1)
   strcpy(format_name,"PCM");
 else if (header.format_type == 6)
  strcpy(format_name, "A-law");
 else if (header.format_type == 7)
  strcpy(format_name, "Mu-law");
 printf("(21-22) Format type: %u %s \n", header.format_type, format_name);
 
read = fread(buffer2, sizeof(buffer2), 1, ptr);
 printf("%u %u \n", buffer2[0], buffer2[1]);

header.channels = buffer2[0] | (buffer2[1] << 8);
printf("(23-24) Channels: %u \n", header.channels);

read = fread(buffer4, sizeof(buffer4), 1, ptr);
 printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);


header.sample_rate = buffer4[0] |
                        (buffer4[1] << 8) |
                        (buffer4[2] << 16) |
                        (buffer4[3] << 24);
 printf("(25-28) Sample rate: %u\n", header.sample_rate);

read = fread(buffer4, sizeof(buffer4), 1, ptr);
 printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

header.byterate  = buffer4[0] |
                        (buffer4[1] << 8) |
                        (buffer4[2] << 16) |
                        (buffer4[3] << 24);

printf("(29-32) Byte Rate: %u , Bit Rate:%u\n", header.byterate, header.byterate*8);

read = fread(buffer2, sizeof(buffer2), 1, ptr);
 printf("%u %u \n", buffer2[0], buffer2[1]);
 
 header.block_align = buffer2[0] |(buffer2[1] << 8);
 printf("(33-34) Block Alignment: %u \n", header.block_align);
read = fread(buffer2, sizeof(buffer2), 1, ptr);
 printf("%u %u \n", buffer2[0], buffer2[1]);
 header.bits_per_sample = buffer2[0] |
                    (buffer2[1] << 8);
 printf("(35-36) Bits per sample: %u \n", header.bits_per_sample);

 read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, ptr);
 printf("(37-40) Data Marker: %s \n", header.data_chunk_header);
 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
 header.data_size = buffer4[0] |
                (buffer4[1] << 8) |
                (buffer4[2] << 16) |
                (buffer4[3] << 24 );
 printf("(41-44) Size of data chunk: %u \n", header.data_size);
 
 // calculate the number of sample
 long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
 printf("Number of samples:%lu \n", num_samples);

long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
 printf("Size of each sample:%ld bytes \n ", size_of_each_sample);
 
 // calculate duration of file
 float duration_in_seconds = (float) header.overall_size / header.byterate;
 printf("Approx.Duration in seconds=%f \n", duration_in_seconds);
 printf("Approx.Duration in h:m:s=%s \n", seconds_to_time(duration_in_seconds));
 
 
 
 	fclose(ptr);
	free(argv[1]);
	return 0;
}

char* seconds_to_time(float raw_seconds) {

  char *hms;

  int hours, hours_residue, minutes, seconds, milliseconds;

  hms = (char*) malloc(100);

  sprintf(hms, "%f", raw_seconds);

  hours = (int) raw_seconds/3600;

  hours_residue = (int) raw_seconds % 3600;

  minutes = hours_residue/60;

  seconds = hours_residue % 60;

  milliseconds = 0;

  // get the decimal part of raw_seconds to get milliseconds

  char *pos;

  pos = strchr(hms, '.');

  int ipos = (int) (pos - hms);

  char decimalpart[15];

  memset(decimalpart, ' ', sizeof(decimalpart));

  strncpy(decimalpart, &hms[ipos+1], 3);

  milliseconds = atoi(decimalpart);

  sprintf(hms, "%d:%d:%d.%d", hours, minutes, seconds, milliseconds);
  return hms;

}



