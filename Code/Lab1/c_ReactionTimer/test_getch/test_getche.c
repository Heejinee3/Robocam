// Include required header files
#include <stdio.h>
// Define function prototype
// Inform the compiler about arguments and return values: conform to actual function in getche.c

char getch(void);
char getche(void);

int main(void)	// Function type of main() is defined also.
{
		char c;	// Set variable of return value from getch()
			int i;

				for(i=0;i<16;i++) {
							c = getch(); // Call getch() and obtain return value of char c
									printf("%c, %d, %xh\n",c,c,c);
										}
					return 0;
}

