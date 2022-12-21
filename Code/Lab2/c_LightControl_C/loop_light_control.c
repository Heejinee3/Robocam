#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "gpio_control.h"

int main(int argc, char *argv[])
{
  int light_id;
  int fd_gpio_30, fd_gpio_31;
  int on_off_int;
  char on_off_str[8];
  int i;
  
  struct timeval s_time, e_time;
  struct timezone tz;
  double start_time, end_time;

  /* 0.Print title */
  printf("loop_light_control\n");

  /* 1. Set variables: light_id = 1; */
  light_id = 1;

  /* 2. Export GPIO 30 & 31 */
  gpio_export(30);
  gpio_export(31);

  /* 3. Set direction of GPIO 30 & 31 to out. */
  gpio_set_dir(30, 1);
  gpio_set_dir(31, 1);

  /* 4. Open GPIO30 & GPIO31. */
  fd_gpio_30 = gpio_fd_open(30);
  fd_gpio_31 = gpio_fd_open(31);

  gettimeofday(&s_time,&tz);
  /* 5. Loop 10times */
  for(i=0; i<10; i++)
  {
    gpio_fd_set_value(fd_gpio_30, 1);
    gpio_fd_set_value(fd_gpio_31, 1);
    gpio_fd_set_value(fd_gpio_30, 0);
    gpio_fd_set_value(fd_gpio_31, 0);
  }
  gettimeofday(&e_time,&tz);
  
  start_time = s_time.tv_sec + s_time.tv_usec*1e-6;
  end_time = e_time.tv_sec + e_time.tv_usec*1e-6;
  printf("start: %.6f end: %.6f\n", start_time, end_time);

  gpio_fd_close(fd_gpio_30);
  gpio_fd_close(fd_gpio_31);
  
  gpio_set_dir(30, 0);
  gpio_set_dir(31, 0);

  gpio_unexport(30);
  gpio_unexport(31);

  return 0;
}
