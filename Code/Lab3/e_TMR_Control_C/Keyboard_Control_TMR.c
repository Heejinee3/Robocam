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
	

  	int ivx = 0;
  	int ivy = 0;
  	int iw = 0;                             //those three to record net pressed times
  	int led_one_on = 0;             //to control LEDs
  	int led_two_on = 0;
  	float vx, vy, wr;                    //entries of vector v
  	float r, G;                             //r is radius of the wheel, G is the gain
  	r = 0.02;
  	G = 30000;
	int g2 = 3000000;
  	
  	float p[3];
  	float w[3];
  	float v[3];
  	float mT[3][3] =  { {0, -0.67, 0.33*0.08}, {-0.577, 0.33, 0.33*0.08}, {0.577, 0.33, 0.33*0.08}}; //the transfrom matrix
  	int input;

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

 	printf(" Key Input Info\n");
 	
 	while (1){
        	input = getchar();
		if (input == 't')         //to quit
		break;
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
					ivx = 0; ivy = 0; iw = 0;  //reset;
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
					gpio_set_value( 31, 0);
					led_one_on = 0;
					}					
					else	
					{
					gpio_set_value( 31, 1);
					led_one_on = 1;
					}

					break;
				case 'e':
					if (led_two_on)		
					{					
					gpio_set_value( 30, 0);
					led_two_on = 0;
					}	
					else	
					{
					gpio_set_value( 30, 1);
					led_two_on = 1;
					}

					break;
		}
		vx = 0.02 * (float)ivx;
    		vy = 0.02 * (float)ivy;
    		wr = 0.01 * (float)iw;
    		printf("v: %f , %f, %f\n", vx, vy, wr);
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
    		printf("w: %f , %f, %f\n", w[0], w[1], w[2]);
    
    		p[0] = /*1510095*/ 1500000 - G * w[0];
    		p[1] = /*1465450*/ 1500000 - G * w[1];
    		p[2] = /*1462500*/ 1500000 - G * w[2];
    		if (p[1] < 1500000){
     			p[1] -= 500000;//39795;
    		}
    		if (p[2] < 150000){
      			p[2] -= 500000;//1434950 45050
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

		printf("p: %f , %f, %f\n", p[0], p[1], p[2]);
    
     
                fprintf(duty0, "%d",(int)p[1] );
                fprintf(run0, "%d", 1);
     		fflush(duty0);
      		fflush(run0);
                fprintf(duty1, "%d", (int)p[2]);
                fprintf(run1, "%d", 1);
     		fflush(duty1);
      		fflush(run1);
      		fprintf(duty2, "%d", (int)p[0]);
                fprintf(run2, "%d", 1);
		fflush(duty2);
      		fflush(run2);
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



