// File gpio_control.h 
// Function definitions for gpio‐control.c and app. 
 
#define MAX_BUF 64     /* For the max length of string */ 
 
int gpio_export(unsigned int gpio);     // gpio means gpio number (0 to 127) 
int gpio_unexport(unsigned int gpio); 
int gpio_set_dir(unsigned int gpio, unsigned int out);  // out = 0: in. out = 1: out. 
 
int gpio_fd_open(unsigned int gpio);    // Returns file descriptor fd 
int gpio_set_value(int fd, unsigned int value);    // value can be 0 or 1 
int gpio_get_value(int fd, unsigned int *value);   // *value will be 0 or 1 
int gpio_fd_close(int fd);
