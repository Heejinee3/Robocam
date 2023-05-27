#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <sys/time.h>
#include <pthread.h>
#include <netdb.h> 
#include "UDP.h"

//Screen dimension constants
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;

//Starts up SDL and creates window
bool init_SDL();

//Frees media and shuts down SDL
void close_SDL();

//Loads individual image
SDL_Surface* loadSurface(void *buf, size_t size);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Current displayed PNG image
SDL_Surface* gJPGSurface = NULL;

//Maximum JPG file size//
size_t FRAME_SIZE = 60000;

//frame buffer
char* frame = NULL;

//UDP socket pointer//
struct UDP_socket *sock_udp;

bool quit = false;

bool init_SDL(void)
{
	//Initialization flag
  bool success = true;
	
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("Lab5", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Initialize JPG loading
			int imgFlags = IMG_INIT_JPG;
			if( !( IMG_Init( imgFlags ) & imgFlags ) )
			{
				printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
				success = false;
			}
			else
			{
				//Get window surface				
        gScreenSurface = SDL_GetWindowSurface(gWindow);
        //Fill the surface white
			  SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0xFF, 0xFF, 0xFF ) );
        //Update the surface
        SDL_UpdateWindowSurface(gWindow);
      }
		}
	}

	return success;
}

void close_SDL()
{
	//Free loaded image
	SDL_FreeSurface( gJPGSurface );
	gJPGSurface = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Surface* loadSurface(void *buf, size_t size)
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
  SDL_RWops *rw_jpg = SDL_RWFromMem (buf, size);
	SDL_Surface* loadedSurface = IMG_Load_RW(rw_jpg, 1);
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image from buffer! SDL_image Error: %s\n", IMG_GetError() );
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, NULL );
		if( optimizedSurface == NULL )
		{
			printf( "Unable to optimize image! SDL Error: %s\n", SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return optimizedSurface;
}

void *thread_viewer (void *vargs)
{
  int n;
  bool isJPG;
  printf("start capturing\n");
  while (!quit) {
    //recv UDP data//
    n = UDP_recv (sock_udp, frame, FRAME_SIZE);
    
    //check JPG file//
    if (n < 4)
      continue;
    isJPG = true;
    
    if (frame[0] != 0xFF) isJPG = false;
    if (frame[1] != 0xD8) isJPG = false;
    if (frame[n-2] != 0xFF) isJPG = false;
    if (frame[n-1] != 0xD9) isJPG = false;

    if (!isJPG)
      continue;

    printf ("recv: %d bytes\n", n);

    gJPGSurface = loadSurface (frame, n);
    SDL_BlitSurface (gJPGSurface, NULL, gScreenSurface, NULL);
    SDL_FreeSurface (gJPGSurface);
    gJPGSurface = NULL;
    SDL_UpdateWindowSurface (gWindow);
  }

  return NULL;
}

int main (int argc, char *argv[])
{ 
  //Control value//
  int ivx = 0;
  int ivy = 0;
  int iw = 0;
  int LL = 0;
  int RL = 0;
  int command_id = 0;
  struct timeval now;
  struct timeval set;
  int sec, msec;
  pthread_t tid;

  float vx, vy, wr;
  float r, G;
  r = 0.02;
  G = 30000;
  int g2 = 3000000;

  float p[3];
  float w[3];
  float v[3];
  float mT[3][3] =  { {0, -0.67, 0.33*0.08}, {-0.577, 0.33, 0.33*0.08}, {0.577, 0.33, 0.33*0.08}}; //the transfrom matrix


  //Control string//
  char pressed_key;
  char str_ctrl[100] = {0};
  char send_str[100] = {0};
  char buffer[100] = {0};
  
  //SDL Event//
  SDL_Event e;
  
  //Init UDP socket//
  sock_udp = UDP_new_socket();
  UDP_socket_init(sock_udp);
  UDP_set_server(sock_udp, 4960);


  //Init SDL socket//
  init_SDL();
  
  //Init frame buffer//
  frame = (char*)malloc(sizeof(char)*FRAME_SIZE);

  pthread_create(&tid, NULL, thread_viewer, NULL);
  gettimeofday(&set, NULL);
  //Event//
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
      else if (e.type == SDL_KEYDOWN) { 
        
        bool pressed = true;

        switch (e.key.keysym.scancode) {
          case SDL_SCANCODE_F12:
            printf ("pressed: F12 (Quit)\n");
            UDP_send (sock_udp, (void *)"End", 4);
            pressed = false;
            quit = true;
            break;

          case SDL_SCANCODE_1:
            pressed_key = '1';
            break;

          case SDL_SCANCODE_2:
            pressed_key = '2';
            break;
  
          case SDL_SCANCODE_3:
            pressed_key = '3';
            break;

          case SDL_SCANCODE_Q:
            pressed_key = 'Q';
            LL ^= 0x01;
            break;

          case SDL_SCANCODE_W:
            pressed_key = 'W';
            ivx++;
            if (ivx > 10)
              ivx = 10;
            break;

          case SDL_SCANCODE_E:
            pressed_key = 'E';
            RL ^= 0x01;
            break;

          case SDL_SCANCODE_A:
            pressed_key = 'A';
            ivy++;
            if (ivy < -10)
              ivy = -10;
            break;

          case SDL_SCANCODE_S:
            pressed_key = 'S';
            ivx = 0;
            ivy = 0;
            iw = 0;
            break;

          case SDL_SCANCODE_D:
            pressed_key = 'D';
            ivy--;
            if (ivy > 10)
              ivy = 10;
            break;

          case SDL_SCANCODE_Z:
            pressed_key = 'Z';
            iw++;
            if (iw > 10)
              iw = 10;
            break;

          case SDL_SCANCODE_X:
            pressed_key = 'X';
            ivx--;
            if (ivx < -10)
              ivx = -10;
            break;

          case SDL_SCANCODE_C:
            pressed_key = 'C';
            iw--;
            if (iw < -10)
              iw = -10;
            break;

          default:
            pressed = false;
        }
        
        if (pressed) {
          command_id++;

          gettimeofday(&now, NULL);
          msec = (now.tv_sec * 1e6 + now.tv_usec) - (set.tv_sec * 1e6 + set.tv_usec);
          msec /= 1000;
          sec = msec / 1000;
          msec = msec % 1000;

          sprintf (str_ctrl, "%d %d.%03d %d %d %d %d %d",
                   command_id, sec, msec, ivx * 10, ivy * 10, iw * 1000, LL, RL);

          vx = 0.02 * (float)ivx*10;
          vy = 0.02 * (float)ivy*10;
          wr = 0.01 * (float)iw*1000;
          
          v[0] = vx;
          v[1] = vy;
          v[2] = wr;
      
          for(int i = 0; i < 3; i++){
            w[i] = 0;
            for(int j = 0; j < 3; j++){
              w[i] += mT[i][j] * v[j];
            }
          }
          
          w[0] /= r;
          w[1] /= r;
          w[2] /= r;
      
          p[0] = /*1510095*/ 1500000 - G * w[0];
          p[1] = /*1465450*/ 1500000 - G * w[1];
          p[2] = /*1462500*/ 1500000 - G * w[2];
          
          if (p[1] < 1500000){
            p[1] -= 500000;
          }
          if (p[2] < 150000){
              p[2] -= 500000;
          }
          if (p[0] < 1500000){
            p[0] -= 500000;
          }
          if (p[1] > 1500000){
              p[1] += 500000;
          }
          if (p[2] > 1500000){
              p[2] += 500000;
          }
          if (p[0] > 1500000){
              p[0] += 500000;
          }
		//  please write your own script using UDP_send
          sprintf(buffer, "%d", command_id);
          strcat(send_str, buffer);
          strcat(send_str, ".");
          sprintf(buffer, "%d", (int)p[1]);
          strcat(send_str, buffer);
          strcat(send_str, ".");
          sprintf(buffer, "%d", (int)p[2]);
          strcat(send_str, buffer);
          strcat(send_str, ".");
          sprintf(buffer, "%d", (int)p[0]);
          strcat(send_str, buffer);
          strcat(send_str, ".");
          sprintf(buffer, "%d", (int)LL);
          strcat(send_str, buffer);
          strcat(send_str, ".");
          sprintf(buffer, "%d", (int)RL);
          strcat(send_str, buffer);

          UDP_send(sock_udp, send_str, strlen(send_str));
		
          printf ("\t\t\t\t\t%s\n", str_ctrl);
          printf ("\t\t\t\t\t%s\n", send_str);

          memset(send_str, 0, sizeof(send_str));
        }
      }
    }
  }
  pthread_join (tid, NULL);

  free (frame);
  frame = NULL;

  close_SDL ();
  UDP_close (sock_udp);

  return 0;
}
