#Embedded Systems Lab1 Testgcc Makefile
# Source file: hello_es.c
# Do everything
all:	nc cc

# Native compile
nc:
	gcc -o listener_pc listener.c
	gcc -o talker_pc talker.c

# Cross compile for Bone Ubuntu
cc:
	arm-linux-gnueabihf-gcc -o listener_bone listener.c
	arm-linux-gnueabihf-gcc -o talker_bone talker.c

# Clean up
clean:
	rm -f hello_es-_pc hello_es
