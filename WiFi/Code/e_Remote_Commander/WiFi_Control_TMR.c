#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <termios.h>
#include "gpio_control_test.h"
static struct termios old_tio;
static struct termios new_tio;

#define MYPORT "4950"	// the port users will be connecting to

#define MAXBUFLEN 100
 
// pre-define listener function
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

 // echo macro code
static void echo(char *str, char *file)
{
  int fd = open(file, O_WRONLY);
  if(fd < 0)
  {
	printf("%s:open error.\n", file);
	exit(-1);
  }
  write(fd, str, strlen(str));
  close(fd);
}
 
int main(int argc, char *argv[])
{
 /*0. Print Title
1. Set control parameters – gain etc.
2. Init PWM sysfs.
3. Init GPIO_LED
4. Open datagram socket and bind*/

  FILE *duty0, *duty1, *duty2, *run0, *run1, *run2;
  
  	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	char* token;
	int token_num = 0;

  	gpio_export(30);	
  	gpio_export(31);
   
  	gpio_set_dir(30, 1);
  	gpio_set_dir(31, 1);

  	if ((duty0 = fopen("/sys/devices/ocp.3/pwm_test_P9_22.15/duty", "w")) == NULL){
    		printf("Error: PWM0 may not have been acquired\n");
    		exit(0);
	}
  	if ((run0 = fopen("/sys/devices/ocp.3/pwm_test_P9_22.15/run", "w")) == NULL){
    		printf("Error: PWM0 may not have been acquired\n");
    		exit(0);
	}
  	if ((duty2 = fopen("/sys/devices/ocp.3/pwm_test_P9_14.16/duty", "w")) == NULL){
    		printf("Error: PWM1 may not have been acquired\n");
    		exit(0);
		}
  	if ((run2 = fopen("/sys/devices/ocp.3/pwm_test_P9_14.16/run", "w")) == NULL){
    		printf("Error: PWM1 may not have been acquired\n");
    		exit(0);
	}
 	if ((duty1 = fopen("/sys/devices/ocp.3/pwm_test_P8_19.17/duty", "w")) == NULL){
    		printf("Error: PWM2 may not have been acquired\n");
    		exit(0);
	}
  	if ((run1 = fopen("/sys/devices/ocp.3/pwm_test_P8_19.17/run", "w")) == NULL){
    		printf("Error: PWM2 may not have been acquired\n");
    		exit(0);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	} 
	
	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}
	
	freeaddrinfo(servinfo);

  /* Print Key guide */
  printf("+-----------------------------------+\n");
  printf("|   Q: LL   |  W: +vx   |   E: RL   |\n");
  printf("|-----------+-----------+-----------|\n");
  printf("|   A: -vy  |  S: Stop  |   D: +vy  |\n");
  printf("|-----------+-----------+-----------|\n");
  printf("|   Z: +w   |  X: -vx   |   C: -w   |\n");
  printf("+-----------------------------------+\n");
	
  while(1)
  {
	// Please refer to attached code ‘listener.c’ for writing your own script (Loop A. ~. B. in Lecture note page. 21.)
	printf("listener: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}
 	
	printf("listener: got packet from %s\n",
		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s));
	printf("listener: packet is %d bytes long\n", numbytes);
	buf[numbytes] = '\0';
	printf("listener: packet contains \"%s\"\n", buf);
 	// use strtok() function to parse command to variables
	// use atoi() function to convert a character string to an integer value
        // Add your own script written in Lab 3 (Loop A. ~. C. in Lecture note page. 20.)
	token = strtok(buf, " ");
	while (token != NULL){
		token_num++;
		token = strtok(NULL, " ");
		if (token_num == 2){
			//printf("%d\n",atoi(token));
            fprintf(duty0, "%d",atoi(token));
	        fprintf(run0, "%d", 1);
 			    fflush(duty0);
      		fflush(run0);
		}
		else if (token_num == 3){
			//printf("%d\n",atoi(token));
            fprintf(duty1, "%d",atoi(token));
	        fprintf(run1, "%d", 1);
     		fflush(duty1);	     				
      		fflush(run1);
		}
		else if (token_num == 4){
			//printf("%d\n",atoi(token));
            fprintf(duty2, "%d",atoi(token));
	        fprintf(run2, "%d", 1);
     		fflush(duty2);
      		fflush(run2);
		}
		else if (token_num == 5){
			//printf("%d\n",atoi(token));
			gpio_set_value( 31, atoi(token));
		}
		else if (token_num == 6){
			//printf("%d\n",atoi(token));
            gpio_set_value( 30, atoi(token));
		}
		
	}
 
	
	token_num = 0;
    usleep(1000);
  }
 
  /* Stop PWM */
   	fprintf(run0, "%d", 0);

   	fprintf(run1, "%d", 0);
	
   	fprintf(run2, "%d", 0);
  /* Close socket*/
	close(sockfd);
  /* Close GPIO_LED*/
   	gpio_unexport(30);
   	gpio_unexport(31);
  /* Close PWM sysfs files*/

   	fclose(run0);
   	fclose(run1);
   	fclose(run2);
	
   	fclose(duty0);
   	fclose(duty1);
   	fclose(duty2);

 
  return 0;
}
