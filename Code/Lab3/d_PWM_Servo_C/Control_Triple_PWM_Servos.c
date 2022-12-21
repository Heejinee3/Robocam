#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
int main()
{
	FILE *duty, *run;
  	int duty_ms[3];
  	int duty_ns[3];
	int d0,d1,d2;
	int i;
	while (1){
    
    		printf("Input Duties (ns): ");
		scanf("%d%d%d",&d0,&d1,&d2);
		duty_ns[0] = d0;
		duty_ns[1] = d1;
		duty_ns[2] = d2;
			/*
    		for ( i = 0; i < 3; i++){
      			duty_ns[i] = duty_ms[i] * 1000000;
      			if (duty_ms[i] < 0){
        		goto finish;
      			}
    		}
			*/
		if ((duty = fopen("/sys/devices/ocp.3/pwm_test_P9_22.15/duty", "w")) == NULL){
			printf("Error: PWM0 may not have been acquired\n");
			exit(0);
			}
      
      		if ((run = fopen("/sys/devices/ocp.3/pwm_test_P9_22.15/run", "w")) == NULL){
			printf("Error: PWM0 may not have been acquired\n");
			exit(0);
			}

      		fprintf(duty, "%d", duty_ns[0]);
      		fprintf(run, "%d", 1);
     		fflush(duty);
      		fflush(run);
		if ((duty = fopen("/sys/devices/ocp.3/pwm_test_P9_14.16/duty", "w")) == NULL){
			printf("Error: PWM1 may not have been acquired\n");
			exit(0);
			}
      
      		if ((run = fopen("/sys/devices/ocp.3/pwm_test_P9_14.16/run", "w")) == NULL){
			printf("Error: PWM1 may not have been acquired\n");
			exit(0);
			}

      		fprintf(duty, "%d", duty_ns[1]);
      		fprintf(run, "%d", 1);
     		fflush(duty);
      		fflush(run);
		if ((duty = fopen("/sys/devices/ocp.3/pwm_test_P8_19.17/duty", "w")) == NULL){
			printf("Error: PWM2 may not have been acquired\n");
			exit(0);
			}
      
      		if ((run = fopen("/sys/devices/ocp.3/pwm_test_P8_19.17/run", "w")) == NULL){
			printf("Error: PWM2 may not have been acquired\n");
			exit(0);
			}

      		fprintf(duty, "%d", duty_ns[2]);
      		fprintf(run, "%d", 1);
     		fflush(duty);
      		fflush(run);
	}

finish:
  
  	if ((run = fopen("/sys/devices/ocp.3/pwm_test_P9_22.15/run", "w")) == NULL){
    		printf("Error: PWM0 may not have been acquired\n");
    		exit(0);
  	}
  	if ((duty = fopen("/sys/devices/ocp.3/pwm_test_P9_22.15/duty", "w")) == NULL){
		printf("Error: PWM0 may not have been acquired\n");
		exit(0);
      	}
	fprintf(run, "%d", 0);
	if ((duty = fopen("/sys/devices/ocp.3/pwm_test_P9_14.16/duty", "w")) == NULL){
		printf("Error: PWM1 may not have been acquired\n");			\
		exit(0);
		}
      
      	if ((run = fopen("/sys/devices/ocp.3/pwm_test_P9_14.16/run", "w")) == NULL){
		printf("Error: PWM1 may not have been acquired\n");
		exit(0);
		}
	fprintf(run, "%d", 0);
	if ((duty = fopen("/sys/devices/ocp.3/pwm_test_P8_19.17/duty", "w")) == NULL){
		printf("Error: PWM2 may not have been acquired\n");
		exit(0);
		}
      
      	if ((run = fopen("/sys/devices/ocp.3/pwm_test_P8_19.17/run", "w")) == NULL){
		printf("Error: PWM2 may not have been acquired\n");
		exit(0);
		}
	fprintf(run, "%d", 0);
	
  	fclose(duty);
  	fclose(run);
}

