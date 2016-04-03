#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
 #include <grp.h>

int main(int argc, char ** argv) {
	/*basic ls:
	  open directory
	  scan directory, storing file names
	  print out file names*/
	struct dirent **names;
	int n;
	FILE *f;
	struct stat buf;
	if (argv[1]==NULL) {
		n=scandir(".", &names, NULL, alphasort);
		if (n<0)
			perror("scandir");
		else {
			//qsort(names, n, sizeof(
			while (n--){
				printf("%s\t", names[n]->d_name);
				free(names[n]);
			}
			free(names);
		}
		printf("\n");
	}
	else if (argv[1]!=NULL) {
		n=scandir(".", &names, NULL, alphasort);
		if (n<0)
			perror("scandir");
		else {
		   char cwd[1024];
		   getcwd(cwd, sizeof(cwd));
					  
		   while (n--) {
			char * filename=names[n]->d_name;
			stat(filename, &buf);
			printf("%s\t", "-");
			if (buf.st_mode->S_IRUSR&1){
			   printf("%s\t", "r-");
			}
			printf("%s\t", getpwuid(buf.st_uid)->pw_name);
			printf("%s\t\t", getgrgid(buf.st_gid)->gr_name);
		   	printf("%ld\t\t", buf.st_size);
			printf("%s\n", names[n]->d_name);
		   }
		}
	}
}
