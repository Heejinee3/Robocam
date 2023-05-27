#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char **argv)

{

        /* Our file descriptor */
	int fd;
	int rc = 0;
	char *rd_buf[16];
	int usr_input=0;
	
	printf("%s: entered\n", argv[0]);

        // Open the device

	fd = open("/dev/LightLEDs", O_RDWR);

	if (fd == -1) {

	perror("open failed");

	rc = fd;

	exit(-1);

	}

	printf("%s: open successful\n", argv[0]);

	while(1){
		printf("Enter data for LED: ");

          //printf("Test 0");

         // write(fd, 0,sizeof(int));

		scanf("%d", &usr_input);

          	if ( usr_input !=0 && usr_input !=1 && usr_input !=2 && usr_input !=3) break;

          //printf("Test 1");


          //printf("Test 4");

		write(fd, &usr_input,sizeof(int));

		}

        //        close(fd);

	return 0;

}
