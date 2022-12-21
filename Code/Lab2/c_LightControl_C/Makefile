# Embedded Bone cross-compile module makefile

default:
	arm-linux-gnueabihf-gcc -o test_light_control test_light_control.c gpio_control.c
	arm-linux-gnueabihf-gcc -o loop_light_control loop_light_control.c gpio_control.c

clean:
	rm test_light_control loop_light_control



