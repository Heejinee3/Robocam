# Embedded Systems Lab1 Testgcc Makefile
# Source file: hello_es.c
# Do everything
all: nc cc
# Native compile
nc:
	gcc -o hello_es_pc hello_es.c
# Cross compile for Bone Ubuntu
cc:
	arm-linux-gnueabihf-gcc -o hello_es hello_es.c
# Clean up
clean:
	rm -f hello_es-_pc hello_es
