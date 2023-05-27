//control_PWM0_Servo.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

int main()
{
	int duty_ms, duty_ns;
	printf("PWM0 Control\n");
	FILE *duty, *run;
  	if ((duty = fopen("/sys/devices/ocp.3/pwm_test_P9_31.10/duty", "w")) == NULL){
    		printf("Error: PWM0 may not have been acquired\n");
    		exit(0);
	}
  	if ((run = fopen("/sys/devices/ocp.3/pwm_test_P9_31.10/run", "w")) == NULL){
    		printf("Error: PWM0 may not have been acquired\n");
    		exit(0);
	}

  	while (1){
    		printf("Input Duty (ms): ");
    		scanf("%d", &duty_ms);
    		if (duty_ms < 0){
     		break;
    		}
    	duty_ns = duty_ms * 1000000;
    	fseek(duty, 0 , SEEK_SET);
    	fseek(run, 0, SEEK_SET);
    	fprintf(duty, "%d", duty_ns);
    	fprintf(run, "%d", 1);
    	fflush(duty);
    	fflush(run);
	}
}
