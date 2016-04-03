#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

char * compute(int choice){
	if (choice==1) {return "rock";}
	else if (choice==2) {return "paper";}
	else if (choice==3) {return "scissors";}
	else {
		perror("invalid choice");
		exit(1);
	}

}

int winner(int c1, int c2) {
	if (c1==c2) { return 3; } //return 3 for tie
	else if (c1==1&&c2==2) { return 2; }
	else if (c1==1&&c2==3) { return 1; }
	else if (c1==2&&c2==1) { return 1; }
	else if (c1==2&&c2==3) { return 2; }
	else if (c1==3&&c2==1) { return 2; }
	else if (c1==3&&c2==2) { return 1; }
	else {
		perror("invalid throw");
		exit(1);
	}
}


void do_parent(int nrounds, int c1r, int c1w, int c2r, int c2w) {
	//fflush(stdout);

	int c1rbytes;
	int choice, choice2;

  /*keep track of totals*/
	int num_ties=0;
	int num_c1=0;
	int num_c2=0;
	
	printf("Beginning %d rounds...\n", nrounds);
	printf("Fight!\n");
	printf("---------------------\n");

	for (int i=0; i<nrounds; i++) {
		int x=1;
		write(c1w, &x, sizeof(int)); //tell child to throw
		write(c2w, &x, sizeof(int)); //tell child to throw

		c1rbytes = read(c1r, &choice, sizeof(choice)); //read their throw
		c1rbytes = read(c2r, &choice2, sizeof(choice)); //read their throw
		int w = winner(choice, choice2); //compute winner
		
		printf("Round %d:\n", i+1);
		printf("Child 1 throws %s!\n", compute(choice)); //return string version of throw
	  printf("Child 2 throws %s!\n", compute(choice2));
	  
		if (w==3){
			printf("Game is a tie!\n");
			num_ties++;
		}
		else if (w==1) {
			printf("Child 1 wins!\n");
			num_c1++;
		} else if (w==2) {
			printf("Child 2 wins!\n");
			num_c2++;
		}
	  printf("---------------------\n");
	}
		printf("Results:\n");//print results
		printf("Child 1: %d\n", num_c1);
		printf("Child 2: %d\n", num_c2);
		printf("Ties: %d\n", num_ties);
		
		if (num_c1==num_c2)
			printf("It's a tie!\n");
	  else if (num_c1>num_c2)
			printf("Child 1 wins!\n");
		else if (num_c2>num_c1)
			printf("Child 2 wins!\n");
}



int main(int argc, char **argv){
	int p2c1[2]; //parent->child1
	int c12p[2]; //child1->parent
	int p2c2[2]; //parent->child2
	int c22p[2]; //child2->parent
	char buf[1024];

	pid_t pid;
	pid_t pid2;

	struct timeval tv;
	gettimeofday(&tv, NULL);

	if (argv[0]==NULL){
		perror("number of rounds not specified");
		exit(1);
	}

	char *p;
	int rounds = strtol(argv[1], &p, 10); //convert argument to int


	if (pipe(p2c1)==-1 || pipe(c12p)==-1 || pipe(p2c2)==-1 || pipe(c22p)==-1){
		perror("pipe");
		exit(1);
	}

	pid = fork();
	if (pid==-1) {
		perror("fork");
		exit(1);

	} else if (pid!=0) { //parent
		pid2=fork();
		

		if (pid==-1) {
			perror("fork");
			exit(1);
		}
		else if (pid2!=0) { //parent
			printf("Child 1 PID: %d\n", pid);
			printf("Child 2 PID: %d\n", pid2);
		
			close(p2c1[0]);
			close(p2c2[0]);
			close(c12p[1]);
			close(c22p[1]);

			do_parent(rounds,c12p[0], p2c1[1], c22p[0], p2c2[1]);

			int x=0;
			write(p2c1[1], &x, sizeof(x)); //tell children to stop
			write(p2c2[1], &x, sizeof(x));
			wait(NULL);
			wait(NULL);
		}
		else if (pid2==0) { //child 2
			close(p2c2[1]);
			close(c22p[0]);
			int rbytes;
			int x;
		
			srand(tv.tv_sec + tv.tv_usec + getpid());
			rbytes = read(p2c2[0], &x, sizeof(x));
			
			while (x==1){ //while told to throw
				int choice=rand() % 3 +1; //random choice
				write(c22p[1], &choice, sizeof(int)); //give parent choice
				read(p2c2[0], &x, sizeof(x)); //read next instruction
			}

		}	
	}
	else { //child 1
		close(p2c1[1]);
		close(c12p[0]);
		int rbytes;
		int x;

		srand(tv.tv_sec + tv.tv_usec + getpid());
		rbytes = read(p2c1[0], &x, sizeof(x));

		while (x==1){ //while told to throw
			int choice=rand() % 3 +1; //random choice
			write(c12p[1], &choice, sizeof(int)); //give parent choice
			read(p2c1[0], &x, sizeof(x)); //read next instruction
		}
	}
	exit(0);
}


