#include <stdio.h>

int main () {
   FILE *fp;
   unsigned char buffer2[2];
   fp = fopen("input.txt","w+");
   fputs("This is tutorialspoint.com", fp);
   int ah;
   ah = fseek( fp, 7, SEEK_SET);
   printf("asdsadPOSITIION%d\n",ah);
   // here the SEEK_SET is the beginning of the file.
   // 7 is the number of bytes from third argument; here the SEEK_SET specify the beginnig of the document.
   fputs(" C Programming", fp);
   ah = fseek(fp,-3,SEEK_CUR);
	fread(buffer2,sizeof(buffer2),1,fp);
	printf("BUFFER:%s",buffer2);
   fclose(fp);
   printf("%s",fp);
   /*
   read = fread(buffer2, sizeof(buffer2), 1, ptr);
   here the buffer2 is an array whose size is 2 bytes. When you call the fread function in the given format
   you get the data into array and you can use it.
   printf("LOOK AT HERE %u %u \n", buffer2[0], buffer2[1]);
   */
   /*
   Burada pointer 7. byte gönderildikten sonra fputs() ile overwrite işlemi gerçekleştiriliyor. 7. byte dan sonra fputs ile
   ne kadar input verilmişse overwrite yapılıyor.
   */
   return(0);
}
