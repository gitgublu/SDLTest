#include"fonctions.h"
#include"starting.h"
#include"loop.h"
#include"mainmenu.h"
#include <iostream>
#include <SDL2/SDL.h>
using namespace std;
int main(int argc,char *argv[]){
	InitResult ir={NULL,NULL,NULL};
	initgame(&ir);
	while(mainmenu(&ir)==0){
	printf("initgame terminated, lunching loop\n");
	printf("renderer:%p\nwindow:%p\nsurface:%p\n",ir.renderer,ir.window,ir.surface);
	loop(&ir);
	}
	SDL_DestroyRenderer(ir.renderer);
	SDL_Quit();
	return 0;
}
