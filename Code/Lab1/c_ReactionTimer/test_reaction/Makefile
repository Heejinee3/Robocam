# Source file: hello_es.c
SRC_FILE_1 = getche.c
SRC_FILE_2 = reaction.c
PC_FILE = reaction_pc
ARM_FILE = reaction

# Do everything
all: nc cc
# Native compile
nc:
	gcc -g -o $(PC_FILE) $(SRC_FILE_1) $(SRC_FILE_2)
# Cross compile for Bone Ubuntu
cc:
	arm-linux-gnueabihf-gcc -o $(ARM_FILE) $(SRC_FILE_1) $(SRC_FILE_2)
# Clean up
clean:
	rm -f $(ARM_FILE) $(PC_FILE)
