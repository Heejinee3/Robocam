#include "gpio_control.h"

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int gpio_export(unsigned int gpio)
{
  int fd;
  char buf[MAX_BUF];

  fd = open("/sys/class/gpio/export", O_WRONLY);
  if(fd < 0){
    perror("Error: gpio_export");
    return fd;
  }
  
  sprintf(buf, "%d", gpio);
  write(fd, buf, strlen(buf));
  close(fd);

  return 0;
}

int gpio_unexport(unsigned int gpio)
{
  int fd;
  char buf[MAX_BUF];

  fd = open("/sys/class/gpio/unexport", O_WRONLY);
  if(fd < 0){
    perror("Error: gpio_unexport");
    return fd;
  }

  sprintf(buf, "%d", gpio);
  write(fd, buf, strlen(buf));
  close(fd);

  return 0;
}

int gpio_set_dir(unsigned int gpio, unsigned int out)
{
  int fd;
  char buf[MAX_BUF];

  sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
  fd = open(buf, O_WRONLY);
  if(fd < 0){
    perror("Error: gpio_set_dir");
    return fd;
  }
  
  if(out == 0)
    write(fd, "in", 2); //Set in direction
  else
    write(fd, "out", 3);  //Set out direction
  close(fd);

  return 0;
}

int gpio_fd_open(unsigned int gpio)
{
  int fd;
  char buf[MAX_BUF];

  sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
  fd = open(buf, O_RDWR | O_NONBLOCK);
  if(fd < 0)
    perror("Error: gpio_fd_open");

  return fd;
}

int gpio_fd_close(int fd)
{
  close(fd);
}

int gpio_fd_set_value(int fd, unsigned int value)
{
  if(value == 0)
    write(fd, "0", 1);
  else
    write(fd, "1", 1);
  
  return 0;
}

int gpio_fd_get_value(int fd, unsigned int *value)
{
  char read_val;

  read(fd, &read_val, 1);

  if(read_val == '0')
    *value = 0;
  else
    *value = 1;

  return 0;
}
