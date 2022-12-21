#Embedded Systems Lab1 Testgcc Makefile
# Source file: hello_es.c
# Do everything
all:	nc cc

# Native compile
nc:
	gcc -o server_pc server.c
	gcc -o client_pc client.c

# Cross compile for Bone Ubuntu
cc:
	arm-linux-gnueabihf-gcc -o server_bone server.c
	arm-linux-gnueabihf-gcc -o client_bone client.c

# Clean up
clean:
	rm -f hello_es-_pc hello_es
