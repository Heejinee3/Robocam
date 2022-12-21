#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

char getch(void);

int main(int argc, char *argv[]) {
	int i;
	char type;
	char reaction;

	struct timeval s_time, e_time;
	struct timezone tz;

	double wait_us;
	
	double reaction_time;
	
	//init rand()//
	srand(time(NULL));
	
	type = 'j';
	for(i=0;i<3;i++) {
		//Random generate time T//
		wait_us = 2.0 + (3.0*rand()/RAND_MAX);
		wait_us *= 1000000;

		usleep(wait_us);
		
		//Random generate char//
		type = rand()%2;
		if(type) type = 'f';
		else type = 'j';

		//Output a string "Type the 'x' charater:"//
		fflush(stdin);
		printf("Type the '%c' character: ", type);
		fflush(stdout);

		//Get Start time//
		gettimeofday(&s_time,&tz);
		//Wait until user hits a key//
		reaction = getch();
		//Get Stop time//
		gettimeofday(&e_time,&tz);
		
		printf("%c\n",reaction);
		
		//Compute Response time//
		reaction_time = (e_time.tv_sec - s_time.tv_sec)*1e3
				+(e_time.tv_usec - s_time.tv_usec)*1e-3;
		
		//Print Key correctness and response time//
		if(type == reaction)
			printf("Correct ");
		else printf("InCorrect ");	
		printf("response %c in %.3fms.\n",reaction,reaction_time);
	}
	return 0;
}
