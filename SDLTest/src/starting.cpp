const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
#include <iostream>
#include <SDL2/SDL.h>
#include"fonctions.h"
using namespace std;
static void animate(SDL_Surface* surface,SDL_Window* window){
	SDL_Surface* img;
	char* tab[7]={"img_presentation/hello_world-6.bmp","img_presentation/hello_world-5.bmp","img_presentation/hello_world-4.bmp","img_presentation/hello_world-3.bmp","img_presentation/hello_world-2.bmp","img_presentation/hello_world-1.bmp","img_presentation/hello_world.bmp"};
	for(int i=0;i<7;i++){
	img=SDL_LoadBMP(tab[i]);
	SDL_BlitSurface(img,NULL,surface,NULL);
	SDL_UpdateWindowSurface( window );
	SDL_Delay(50);
	}
	SDL_Delay(1500);
	for(int i=5;i>-1;i--){
	img=SDL_LoadBMP(tab[i]);
	SDL_BlitSurface(img,NULL,surface,NULL);
	SDL_UpdateWindowSurface( window );
	SDL_Delay(50);
	}
	img=SDL_LoadBMP("utiliser_les_fleches.bmp");
	SDL_BlitSurface(img,NULL,surface,NULL);
	SDL_UpdateWindowSurface( window );
	SDL_Event event;
	SDL_Event* e=&event;
	do{
		SDL_PollEvent(e);
	}while(e->key.keysym.sym!=SDLK_UP && e->key.keysym.sym!=SDLK_DOWN && e->key.keysym.sym!=SDLK_LEFT && e->key.keysym.sym!=SDLK_RIGHT);
	SDL_FreeSurface(img);
}
InitResult* initgame(InitResult* ir){
	if(SDL_Init(SDL_INIT_VIDEO)<0){exit(0);}
	ir->window=SDL_CreateWindow( "mygame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if(ir->window==NULL){exit(0);}
	ir->renderer = SDL_CreateRenderer( ir->window, -1, SDL_RENDERER_ACCELERATED );
	ir->surface=SDL_GetWindowSurface( ir->window );
	printf("starting animate");
	animate(ir->surface,ir->window);
	printf("Finish animate");
	return ir;
}

