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
#include <sys/time.h>
#include "getche.h"

#define SERVERPORT "4950"   // the port users will be connecting to
 
// initialize variables
 
int main(int argc, char *argv[])
{
  
// initialize variables and error print
    int sockfd;
    struct addrinfo hints, *servinfo, *p1;
    int rv;
    int numbytes;
    char send_str[100] = {0};
    char buffer[100];
    int cmd_num = 0;
    struct timeval init_t;
    struct timeval current_t;
    double elapsed_t;

  	int ivx = 0;
  	int ivy = 0;
  	int iw = 0;                         
  	int led_one_on = 0;       
  	int led_two_on = 0;
  	float vx, vy, wr;                  
  	float r, G;                        
  	r = 0.02;
  	G = 30000;
	int g2 = 3000000;
  	
  	float p[3];
  	float w[3];
  	float v[3];
  	float mT[3][3] =  { {0, -0.67, 0.33*0.08}, {-0.577, 0.33, 0.33*0.08}, {0.577, 0.33, 0.33*0.08}}; //the transfrom matrix
  	int input;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    
// Get argument of destination IP (argv) of TMR
    if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    	return 1;
    }
    
// Init datagram socket.
	for(p1 = servinfo; p1 != NULL; p1 = p1->ai_next) {
		if ((sockfd = socket(p1->ai_family, p1->ai_socktype,
				p1->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	}

	if (p1 == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	} 
	
  printf("Remote_Commander_PC starts...now!\n");
  printf("Key input menu (without enter)\n");
  printf("+-----------------------------------+\n");
  printf("|   Q: LL   |  W: +vx   |   E: RL   |\n");
  printf("|-----------+-----------+-----------|\n");
  printf("|   A: -vy  |  S: Stop  |   D: +vy  |\n");
  printf("|-----------+-----------+-----------|\n");
  printf("|   Z: +w   |  X: -vx   |   C: -w   |\n");
  printf("+-----------------------------------+\n");
  printf("Speed up/down with multiple key strokes\n");
  printf("'T' key terminates.\n");
  
  gettimeofday(&init_t, NULL);

  while(1)

// Loop start

  {
	// Add your own script written in Lab 3 (Loop A. ~. C. in Lecture note page. 20.)
        input = getch();
		if (input == 't')         
    		goto done;
		switch (input){
				case 'a':
					ivy += 10;
					break;
				case 'd':
					ivy -= 10;
					break;
				case 'w':
					ivx += 10;
					break;
				case 'x':
					ivx -= 10;
					break;
				case 's':
					ivx = 0; ivy = 0; iw = 0;  
					break;
				case 'z':
					iw += 1000;
					break;
				case 'c':
					iw -= 1000;
					break;
				case 'q':
					if (led_one_on) 	
					{
					led_one_on = 0;
					}					
					else	
					{
					led_one_on = 1;
					}

					break;
				case 'e':
					if (led_two_on)		
					{					
					led_two_on = 0;
					}	
					else	
					{
					led_two_on = 1;
					}

					break;
		}
    		vx = 0.02 * (float)ivx;
    		vy = 0.02 * (float)ivy;
    		wr = 0.01 * (float)iw;
    		
    		v[0] = vx;
    		v[1] = vy;
    		v[2] = wr;
    
    		for(int i = 0; i < 3; i++){
      			w[i] = 0;
      			for(int j = 0; j < 3; j++){
				w[i] += mT[i][j] * v[j];
      			}
    		}
    		
    		w[0] /= r;
    		w[1] /= r;
    		w[2] /= r;
    
    		p[0] = /*1510095*/ 1500000 - G * w[0];
    		p[1] = /*1465450*/ 1500000 - G * w[1];
    		p[2] = /*1462500*/ 1500000 - G * w[2];
    		
    		if (p[1] < 1500000){
     			p[1] -= 500000;
    		}
    		if (p[2] < 150000){
      			p[2] -= 500000;
    		}
    		if (p[0] < 1500000){
     			p[0] -= 500000;
    		}
    		if (p[1] > 1500000){
      			p[1] += 500000;
    		}
    		if (p[2] > 1500000){
      			p[2] += 500000;
    		}
    		if (p[0] > 1500000){
      			p[0] += 500000;
    		}

	gettimeofday(&current_t, NULL);
	elapsed_t = (current_t.tv_sec - init_t.tv_sec) + (current_t.tv_usec - init_t.tv_usec)/1000000.0;

  	// Please refer to attached code ‘talker.c’ for writing your own script (Loop D. ~. E. in Lecture note page. 20.)

	sprintf(buffer,"%d",cmd_num);
	strcat(send_str, buffer);
	strcat(send_str, " ");
	sprintf(buffer,"%.3f",elapsed_t);
	strcat(send_str, buffer);
	strcat(send_str, " ");
	sprintf(buffer,"%d",(int)p[1]);
	strcat(send_str, buffer);
	strcat(send_str, " ");
	sprintf(buffer,"%d",(int)p[2]);
	strcat(send_str, buffer);
	strcat(send_str, " ");
	sprintf(buffer,"%d",(int)p[0]);
	strcat(send_str, buffer);
	strcat(send_str, " ");
	sprintf(buffer,"%d",(int)led_one_on);
	strcat(send_str, buffer);
	strcat(send_str, " ");
	sprintf(buffer,"%d",(int)led_two_on);
	strcat(send_str, buffer);
	
	if ((numbytes = sendto(sockfd, send_str, strlen(send_str), 0,
			 p1->ai_addr, p1->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}
 
  	// Print information: key and cmd.
    printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
    printf("key: \'%c\'. cmd: %s\n", input, send_str);
	
	cmd_num += 1;
	memset(send_str, 0, 100*sizeof(char));

	usleep(1000);
  }
 
done:
  freeaddrinfo(servinfo);
  close(sockfd);
  
  return 0;
}
