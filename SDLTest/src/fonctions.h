#include <iostream>
#include <SDL2/SDL.h>
#ifndef _FONCTIONS_H_
#define _FONCTIONS_H_
typedef struct InitResult{
	SDL_Window* window;
	SDL_Surface* surface;
	SDL_Renderer* renderer;
}InitResult;
SDL_Texture* loadTexture( std::string path , SDL_Renderer* renderer,int* w,int* h);
void turntoangle(double* angle, double otherangle, double increment);
double vecttoangle(int vectorx,int vectory);
double distint(double fromx, double fromy, double tox, double toy);
void shortervector(double lengh, double posx, double posy, double* shorterx, double* shortery);
Uint32 getpixel(SDL_Surface *surface, int x, int y);
bool amiin(double mex, double mey, double itx, double ity, double w, double h);
void readlineinfile(FILE* file,char* chtab);
#endif
