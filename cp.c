#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int main(int argc, char **argv){
#if 0
   struct stat buf;
   if (stat(argv[1], &buf)

#endif


   struct stat buf;
   if (stat(argv[1], &buf)==-1){ //source file doesn't exist
	exit(0);
   }
   else {
	//create new file
	//open first file
	//read first file, write to second
	//close
	if (stat(argv[2], &buf)==0) {
        	//delete file and copy over
        	char *filename = argv[2];
        	remove(filename);
	}
	int c;
	FILE *fp1;
	FILE *fp2;
	fp1=fopen(argv[1], "r");
	fp2=fopen(argv[2], "w");
	int x;
	while ( (x=fgetc(fp1))!=EOF) {
	  fputc(x, fp2);
	} 
	fclose(fp1);
	fclose(fp2); 
    }    
}

