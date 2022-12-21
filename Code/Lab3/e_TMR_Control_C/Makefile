# Embedded Systems Lab2 

# Do everything
all:	cg ct
# Native compile
cg:	
	arm-linux-gnueabihf-gcc -c -o gpio_int_test.o gpio_int_test.c
# Cross compile for Bone Ubuntu
ct:
	arm-linux-gnueabihf-gcc -o test_light_control Keyboard_Control_TMR.c gpio_int_test.o

# Clean up
clean:
	rm -f test_light_control
