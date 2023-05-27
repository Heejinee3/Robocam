# Source file: hello_es.c
# Do everything
all: nc cc
# Native compile
nc:
	gcc -o test_gettimeofday_usleep_pc test_gettimeofday_usleep.c
# Cross compile for Bone Ubuntu
cc:
	arm-linux-gnueabihf-gcc -o test_gettimeofday_usleep test_gettimeofday_usleep.c
# Clean up
clean:
	rm -f test_gettimeofday_usleep test_gettimeofday_usleep_pc
