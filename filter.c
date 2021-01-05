#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0


// WAVE file header format
struct HEADER {

    unsigned char riff[4];                      // RIFF string
	//unsigned char	1 byte
    unsigned int overall_size   ;               // overall size of file in bytes
	// unsigned int	2 or 4 bytes
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
struct HEADER header;
unsigned char buffer4[4];
unsigned char buffer2[2];
char* seconds_to_time(float seconds);
int read = 0;
/*

fread(header.wave, sizeof(header.wave), 1, ptr);
header.wave is the array that the data is written to. 
sizeof(header.wave) : number of byte that the pointer will move to. 
1 is the each element has 1 byte size.
ptr is the pointer from where all the data is readed.
*/
int main(int argc, char **argv) {
	
	if(argc <2){
			printf("No wave file specified");
			return 0;
	}
	//printf("%s",argv[1]);
	// OPEN FILE
	ptr = fopen(argv[1],"rb+");
	// This function returns a FILE pointer
	if (ptr == NULL){
		printf("Error opening file");
		exit(1);
	}
	// START READING
	
	read = fread(header.riff,sizeof(header.riff),1,ptr);
	// 1-4 byte
	printf("%s\n",header.riff);
	
	// %u is an unsigned integer.
	read = fread(buffer4, sizeof(buffer4), 1, ptr);
	// 5-8 byte
 	printf("Chunk Size : (Liddle Endian):\t%u\t%u\t%u\t%u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
	// convert little endian to big endian 4 byte int.
	header.overall_size  = buffer4[0] |
                        (buffer4[1]<<8) |
                        (buffer4[2]<<16) |
                        (buffer4[3]<<24);
    printf("(5-8) Overall size: bytes:%u, Kb:%u \n", header.overall_size, header.overall_size/1024);
    
 	read = fread(header.wave, sizeof(header.wave), 1, ptr);
 	// 1 byte 9-12 since it is 4 bytes. 
 	printf("(9-12) Wave marker: %s\n", header.wave);

	read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, ptr);
	printf("(13-16) Fmt marker: %s\n", header.fmt_chunk_marker);
	// here the fmt_chunk_marker is 4 bytes, that is why fread will move the pointer 4 bytes more.
	
	read = fread(buffer4, sizeof(buffer4), 1, ptr);
	printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
	// again we are using the same buffer for parsing 4 bytes data.
	
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
 printf("LOOK AT HERE %u %u \n", buffer2[0], buffer2[1]);
 
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
 
 /*
 int channel_size = ((int)header.data_size)/2;
 unsigned char left_channel[channel_size];
 unsigned char right_channel[channel_size];
 fread(left_channel, sizeof(channel_size), 1, ptr);
 fread(right_channel, sizeof(channel_size), 1, ptr);
 printf("%u",left_channel);
 fseek(ptr,44,SEEK_SET);
 */
 // 44 den başlayıp sample sayısının yarısını al ve üzerine yaz geri kalanı sağ channel da al yaz .
 
 
 // calculate the number of sample
 long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
 printf("Number of samples:%lu \n", num_samples);

long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
 printf("Size of each sample:%ld bytes \n ", size_of_each_sample);
 
 // calculate duration of file
 float duration_in_seconds = (float) header.overall_size / header.byterate;
 printf("Approx.Duration in seconds=%f \n", duration_in_seconds);
 printf("Approx.Duration in h:m:s=%s \n", seconds_to_time(duration_in_seconds));
 long bytes_in_each_channel = (size_of_each_sample / header.channels);
 printf("BYTES IN EACH CHANNEL%ld\n",bytes_in_each_channel);
 // read each sample from data chunk if PCM
 if (header.format_type == 1) { // PCM
    printf("Dump sample data? Y/N?");
    char c = 'n';
    scanf("%c", &c);
    if (c == 'Y' || c == 'y') {
        long i =0;
        char data_buffer[size_of_each_sample];
        int  size_is_correct = TRUE;
        // make sure that the bytes-per-sample is completely divisible by num.of channels
        long bytes_in_each_channel = (size_of_each_sample / header.channels);
        printf("BYTES IN EACH CHANNEL%ld",bytes_in_each_channel);
        if ((bytes_in_each_channel  * header.channels) != size_of_each_sample) {
            printf("Error: %ld x %ud <> %ld\n", bytes_in_each_channel, header.channels, size_of_each_sample);
            size_is_correct = FALSE;
        }
        if (size_is_correct) {
                    // the valid amplitude range for values based on the bits per sample
            long low_limit = 0l;
            long high_limit = 0l;
            switch (header.bits_per_sample) {
                case 8:
                    low_limit = -128;
                    high_limit = 127;
                    break;
                case 16:
                    low_limit = -32768;
                    high_limit = 32767;
                    break;
                case 32:
                    low_limit = -2147483648;
                    high_limit = 2147483647;
                    break;
            }                  
            //printf("nn.Valid range for data values : %ld to %ld \n", low_limit, high_limit);
            short channel_right[bytes_in_each_channel];
            short channel_left[bytes_in_each_channel];
            for (i =1; i <= num_samples; i++) {
            	// Each sample has 4 bytes and first two bytes for channel 0 and the second two bytes
            	// for the channel 1, I need to change this two bytes each time with one another.
                //printf("==========Sample %ld / %ld=============\n", i, num_samples);
                read = fread(data_buffer, sizeof(data_buffer), 1, ptr);
                // data_buffer has size of each sample and each sample has two channel which are 2bytes for each channel
                // in our case.
				if (read == 1) {
                    // dump the data read
                    unsigned int  xchannels = 0;
                    int data_in_channel = 0;
                    int data_in_channel_1 = 0;
                    int offset = 0; // move the offset for every iteration in the loop below
					//for (xchannels = 0; xchannels < header.channels; xchannels ++ ) {
                        //printf("Channel#%d : ", (xchannels+1));
                        // convert data from little endian to big endian based on bytes in each channel sample
                        if (bytes_in_each_channel == 4) {					
							 read = fseek(ptr,-1*sizeof(data_buffer),SEEK_CUR); 
							 data_in_channel = (data_buffer[0] & 0x00ff) |
                                                ((data_buffer[1] & 0x00ff) <<8) |
                                                ((data_buffer[2] & 0x00ff) <<16) |
                                                (data_buffer[3]<<24);      
							  printf("%d\n",data_in_channel);                	 
							 fread(channel_right, sizeof(channel_right), 1, ptr);
							 fread(channel_left, sizeof(channel_left), 1, ptr);
							 read = fseek(ptr,-2*bytes_in_each_channel,SEEK_CUR);
							 read = fwrite(channel_left , 1 , sizeof(channel_left) ,ptr);
							 read = fwrite(channel_right , 1 , sizeof(channel_right) ,ptr);
							 //printf("%d",read);
							 read = fread(data_buffer, sizeof(data_buffer), 1, ptr);
							data_in_channel = (data_buffer[0] & 0x00ff) |
                                                ((data_buffer[1] & 0x00ff) <<8) |
                                                ((data_buffer[2] & 0x00ff) <<16) |
                                                (data_buffer[3]<<24);
                            read = fseek(ptr,-1*sizeof(data_buffer),SEEK_CUR); 
                            printf("%d\n",data_in_channel);  
                        }
                        else if (bytes_in_each_channel == 2) {

                            data_in_channel_1 = (data_buffer[0] & 0x00ff) |
                                                (data_buffer[1] << 8);
							//printf("SAMPLE:%d:::%d\n",i,data_in_channel);                	 
                        	 read = fseek(ptr,-1*sizeof(data_buffer),SEEK_CUR); 
							 read = fread(channel_right, sizeof(channel_right), 1, ptr);
							 read = fread(channel_left, sizeof(channel_left), 1, ptr);
							 read = fseek(ptr,-2*bytes_in_each_channel,SEEK_CUR);
							 read = fwrite(channel_left , 1 , sizeof(channel_left) ,ptr);
							 read = fwrite(channel_right , 1 , sizeof(channel_right) ,ptr);
							 read = fseek(ptr,-2*bytes_in_each_channel,SEEK_CUR);
                        	//printf("%d",read);
                        	read = fread(data_buffer, sizeof(data_buffer), 1, ptr);
                            data_in_channel = (data_buffer[0] & 0x00ff) |
                                                (data_buffer[1] << 8);
                            //read = fseek(ptr,-1*sizeof(data_buffer),SEEK_CUR); 
                            //printf("SAMPLE:%d:::%d\n",i,data_in_channel_1);
							if (data_in_channel != data_in_channel_1){
								printf("SAMPLE:%d:Channel_0::%d::::Channel_1:::%d\n",i,data_in_channel,data_in_channel_1);
							}  
                            
                        }
                        offset += bytes_in_each_channel;       
                        //printf("%d\n", data_in_channel);
                        // check if value was in range
                        if (data_in_channel < low_limit || data_in_channel > high_limit)
                            printf("**value out of range\n");
                        //printf(" | ");
                    //}
                    //printf("\n");
                }
                else {
                    printf("Error reading file. %d bytesn", read);
                    break;
                }
            } //    for (i =1; i <= num_samples; i++) {
        } //    if (size_is_correct) {
     } // if (c == 'Y' || c == 'y') {
 } //  if (header.format_type == 1) {



 
 
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


