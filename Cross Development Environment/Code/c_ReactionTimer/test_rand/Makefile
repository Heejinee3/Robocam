# Source file: test_rand.c
# Do everything
all: nc cc
# Native compile
nc:
	gcc -o test_rand_pc test_rand.c
# Cross compile for Bone Ubuntu
cc:
	arm-linux-gnueabihf-gcc -o test_rand test_rand.c
# Clean up
clean:
	rm -f test_rand test_rand_pc
