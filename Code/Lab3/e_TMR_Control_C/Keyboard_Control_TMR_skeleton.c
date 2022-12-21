#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <termios.h>
#include <unistd.h>
#include "gpio_control.h"
static struct termios old_tio;
static struct termios new_tio;

int main(void){
	FILE *duty0, *duty1, *duty2, *run0, *run1, *run2;
	
	// [ADD your own script: initialize variables (ivs, ivy, iw, RL, LL, vx, vy, wr, r, G, p, w, v, matrix T, etc.)]
  	
  	gpio_export(30);	
  	gpio_export(31);
   
  	gpio_set_dir(30, 1);
  	gpio_set_dir(31, 1);
	
	// [ADD your own script: PWM port file open ]
	// example for pwm_test_P9_31.10
  	if ((duty0 = fopen("/sys/devices/ocp.3/pwm_test_P9_31.10/duty", "w")) == NULL){
    		printf("Error: PWM0 may not have been acquired\n");
    		exit(0);
	}
	

 	printf(" Key Input Info\n");
 	
 	while (1){
        	input = getchar();
		if (input == 't')         //to quit
		break;
		switch (input){
				case 'a':
					//[ADD your own script]
					break;
				case 'd':
					//[ADD your own script]
					break;
				case 'w':
					//[ADD your own script]
					break;
				case 'x':
					//[ADD your own script]
					break;
				case 's':
					//[ADD your own script]
					break;
				case 'z':
					//[ADD your own script]
					break;
				case 'c':
					//[ADD your own script]
					break;
				case 'q':
					if (led_one_on) 	
					{
					//[ADD your own script]
					}					
					else	
					{
					//[ADD your own script]
					}

					break;
				case 'e':
					if (led_two_on)		
					{					
					//[ADD your own script]
					}	
					else	
					{
					//[ADD your own script]
					}

					break;
		}
		
		//[ADD your own script: convert ivx, ivy, iw to vx, vy, and wr]
    		
		//[ADD your own script: convert vx,vy, and w to wheel velocity using Kinematics]
		
		//[ADD your own script: compensate deadband]
    
		//[ADD your own script: Actuate PWMs three PWM duty value (in ns)]
		// Example code
			fprintf(duty0, "%d",(int)p[1] );
			fprintf(run0, "%d", 1);
     		fflush(duty0);
      		fflush(run0);
	}
	
   	fprintf(run0, "%d", 0);
   	fprintf(run1, "%d", 0);	
   	fprintf(run2, "%d", 0);
	
   	gpio_unexport(30);
   	gpio_unexport(31);
		
   	fclose(run0);
   	fclose(run1);
   	fclose(run2);
	
   	fclose(duty0);
   	fclose(duty1);
   	fclose(duty2);
   	return 0;
	}	



