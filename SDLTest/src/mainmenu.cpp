#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include"fonctions.h"
typedef struct Button {
	int textureindex;
	int x;
	int y;
}Button;
int mainmenu(InitResult* ir){
	SDL_Texture* tabtextures[5];
	Button tabbutton[2]={{3,320,200},{4,320,300}};
	bool optionselected=false;
	char* tabtexturespath[5]={"mainmenu/mainmenu.bmp","mainmenu/mainmenu-1.bmp","mainmenu/mainmenu-2.bmp","mainmenu/jouer.bmp","mainmenu/fermer.bmp"};
	for(int i=0;i<5;i++){
		tabtextures[i]=loadTexture(tabtexturespath[i],ir->renderer,NULL,NULL);
	}
	int order[4]={0,1,2,1};
	int indexorder=0;
	int buttonselected=0;
	while(!optionselected){
		SDL_SetRenderDrawColor(ir->renderer, 0, 0, 0, 255);
		SDL_RenderClear(ir->renderer);
		indexorder++;
		if(indexorder>3){
			indexorder=0;
		}
		SDL_RenderCopy(ir->renderer,tabtextures[order[indexorder]],NULL,NULL);
		for(int i=0;i<2;i++){
			int w;
			int h;
			if(i==buttonselected){w=400;h=80;}else{w=200,h=40;}
			SDL_Rect dist={tabbutton[i].x-(w/2),tabbutton[i].y-(h/2),w,h};
			SDL_RenderCopy(ir->renderer,tabtextures[tabbutton[i].textureindex],NULL,&dist);
		}
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_KEYDOWN){
				switch (e.key.keysym.sym){
				case SDLK_UP:
					buttonselected--;
					break;
				case SDLK_DOWN:
					buttonselected++;
					break;
				case SDLK_RETURN:
					optionselected=true;
					break;
				}
			}
		}
		if(buttonselected>1){
			buttonselected-=2;
		}
		if(buttonselected<0){
			buttonselected+=2;
		}
		SDL_RenderPresent(ir->renderer);
		SDL_Delay(100);
	}
	return buttonselected;
}



