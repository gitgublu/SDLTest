#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include"fonctions.h"
#include"loop.h"
#define O enty->
#define NBTEXTURE 50
#define NBENTITY 50
#define NBPLANET 50
#define NBDEFAULTTEXTURE 8
#define ENTY tabentity[i]
static SDL_Texture* tabtextures [NBTEXTURE];
static SDL_Texture* tabdefaulttextures [NBDEFAULTTEXTURE];
static Entity tabentity[NBENTITY];
static Planet tabplanets[NBPLANET];
static Explosion tabexplosion[NBENTITY];
static Ui ui;
static int arenaw, arenah;
static bool died=false;
static bool win=false;
LevelType leveltype;
int x;
int y;
int tickcounter=0;
int delay=100;
static Entity* createentity(Entity enty){
	for(int i=0;i<NBENTITY;i++){
		if(tabentity[i].existing==false){
			tabentity[i]=enty;
			return &tabentity[i];
		}
	}
	return NULL;
}

static Explosion* createexplosion(TextureAnimated ta,Vector pos){
	for(int i=0;i<NBENTITY;i++){
		if(tabexplosion[i].existing==false){
			tabexplosion[i].texture=ta;
			tabexplosion[i].pos=pos;
			tabexplosion[i].existing=true;
			return &tabexplosion[i];
		}
	}
	return NULL;
}

static Entity* createplanet(Planet enty){
	for(int i=0;i<NBPLANET;i++){
		if(tabplanets[i].existing==false){
			tabplanets[i]=enty;
			if(!(enty.fix)){
				return createentity({PLANET,{false,0},enty.texture,{6,6,6,NULL,true},enty.w,enty.h,0,enty.pos,{0,0},0,i,0,true}); // @suppress("Invalid arguments")
			}
			i=NBPLANET;
		}
	}
	return NULL;
}

static void traitetextureanimated(TextureAnimated* tex){
	if(tex->actualindex< tex->indexmax){
		tex->actualindex++;
		}else{
			tex->actualindex=tex->indexmin;
		}
	if(tex->isdefaulttable){tex->texture = tabdefaulttextures[tex->actualindex];}else{tex->texture = tabtextures[tex->actualindex];}
}

static void traitecolliding(InitResult* ir,Entity* enty){
	double smallest = distint(enty->pos.x,enty->pos.y,tabplanets[0].pos.x,tabplanets[0].pos.y)-sqrt((tabplanets[0].w)*(tabplanets[0].w+tabplanets[0].h*tabplanets[0].h))
			,smallestindex = 0;
	for(int i=1;i<NBPLANET;i++){
		if(((enty->type!=PLANET)||(enty->planetindex!=i)) && tabplanets[i].existing){
			double diam=sqrt((tabplanets[i].w)*(tabplanets[i].w+tabplanets[i].h*tabplanets[i].h));
			double actual=distint(enty->pos.x,enty->pos.y,tabplanets[i].pos.x,tabplanets[i].pos.y)-diam;
			if(actual<smallest)
			{smallest=actual;smallestindex=i;}
		}
	}
	enty->indexactualplanet=smallestindex;

	Planet* planet=&tabplanets[enty->indexactualplanet];
	if(planet->existing && amiin(enty->pos.x,enty->pos.y,planet->pos.x-(planet->w/2),planet->pos.y-(planet->h/2),planet->w,planet->h)){
		{
			double posx=enty->pos.x-(planet->pos.x-(planet->w/2));
			double posy=enty->pos.y-(planet->pos.y-(planet->h/2));
			SDL_Surface* srf=planet->groundsurfacepath;
			Uint32 pixel=getpixel(srf,posx,posy);
			SDL_Color RGB;
			Uint8 alpha;
			SDL_GetRGBA(pixel,srf->format,&(RGB.r),&(RGB.g),&(RGB.b),&alpha);
			if(alpha==255){
				if(sqrt((enty->velocity.x*enty->velocity.x)+(enty->velocity.y*enty->velocity.y))>planet->maxspeedbeforeexplosion){
					printf("boom");
					enty->existing=false;
					if(enty->type==PLANET){tabplanets[enty->planetindex].existing=false;}
					createexplosion(enty->explosiontexture,enty->pos);
				}else{
					enty->pos.x-=enty->velocity.x;
					enty->pos.y-=enty->velocity.y;
					enty->velocity.x=-(enty->velocity.x);
					enty->velocity.y=-(enty->velocity.y);
				}
			}
		}
	}
}

static void traiteexplosion(Explosion* explosion,InitResult* ir){
	if(explosion->incrasesize){
		explosion->size+=0.1;
		if(explosion->size>2){
			explosion->incrasesize=false;
		}
	}else{
		explosion->size-=0.1;
	}
	SDL_Rect rect={(int)(explosion->pos.x-x-(explosion->size*60)),(int)(explosion->pos.y-y-(explosion->size*60)),(int)(explosion->size*60),(int)(explosion->size*60)};
	SDL_RenderCopy(ir->renderer,tabdefaulttextures[6],NULL,&rect);
}

static void traiteplanet(Planet* planet,int planetindex,InitResult* ir,int mapposx,int mapposy){
	traitetextureanimated(&(planet->texture));
	SDL_Rect dist={(int)(((planet->pos.x) - mapposx)-(planet->w/2)),(int)(((planet->pos.y) - mapposy)-(planet->h/2)),planet->w,planet->h};
	SDL_RenderCopy(ir->renderer,planet->texture.texture,NULL,&dist);
	for(int i=0;i<NBENTITY;i++){
			if(ENTY.existing && ((tabentity[i].type!=PLANET)||(tabentity[i].planetindex != planetindex))){
				double d=distint(ENTY.pos.x,ENTY.pos.y,planet->pos.x,planet->pos.y);
				double a= 700.00*(planet->mass/(d*d));
				double addx, addy;
				if(a<d){
				shortervector(a,(planet->pos.x-ENTY.pos.x),(planet->pos.y-ENTY.pos.y),&addx,&addy);
				}else{
					addx=(planet->pos.x-ENTY.pos.x);
					addy=(planet->pos.y-ENTY.pos.y);
				}
				ENTY.velocity.x+=(addx/10);
				ENTY.velocity.y+=(addy/10);
			}
	}
}

static void traitentity(Entity* enty,InitResult* ir,int mapposx,int mapposy){
	O pos.x+=O velocity.x;
	O pos.y+=O velocity.y;
	traitecolliding(ir,enty);
if(enty->existing){
	traitetextureanimated(&(O texture));
	SDL_Rect dist={(int)(((enty->pos.x) - mapposx)-(enty->w/2)),(int)(((enty->pos.y) - mapposy)-(enty->h/2)),enty->w,enty->h};
	SDL_RenderCopyEx(ir->renderer,enty->texture.texture,NULL,&dist,enty->angle,NULL,SDL_FLIP_NONE);
}
}

static void winanim(InitResult* ir){
	int winw,winh;
	int texw,texh;
	SDL_GetWindowSize(ir->window,&winw,&winh);
	SDL_Texture* vous=loadTexture("vousavezgagne/vous.bmp",ir->renderer,&texw,&texh);
	SDL_Rect dist={(winw-texw)/2,(winh-texh)/2,texw,texh};
	SDL_RenderCopy(ir->renderer,vous,NULL,&dist);
	SDL_RenderPresent(ir->renderer);
	SDL_Delay(500);
	vous=loadTexture("vousavezgagne/avez.bmp",ir->renderer,&texw,&texh);
	dist={(winw-texw)/2,(winh-texh)/2,texw,texh};
	SDL_RenderCopy(ir->renderer,vous,NULL,&dist);
	SDL_RenderPresent(ir->renderer);
	SDL_Delay(500);
	vous=loadTexture("vousavezgagne/gagne.bmp",ir->renderer,&texw,&texh);
	dist={(winw-texw)/2,(winh-texh)/2,texw,texh};
	SDL_RenderCopy(ir->renderer,vous,NULL,&dist);
	SDL_RenderPresent(ir->renderer);
	SDL_Delay(500);

}

static void actionwin(InitResult* ir,Entity* enty, bool* Exit){
	if(leveltype==CAPTURETHEWINLEVEL){
		for(int i=0;i<NBENTITY;i++){
			if(ENTY.existing){
				if(amiin(ENTY.pos.x,ENTY.pos.y,enty->pos.x-(enty->w/2),enty->pos.y-(enty->h/2),enty->w,enty->h)){
					if(ENTY.player.drivenbyplayer){
						winanim(ir);
						(*Exit)=true;
						break;
					}
				}
			}
		}
	}
}

static void actionplanet(Entity* enty){
	tabplanets[enty->planetindex].pos.x = (enty->pos.x);
	tabplanets[enty->planetindex].pos.y = (enty->pos.y);
}

static void actionspaceship(Entity* enty,InitResult* ir,Events* e,double inc){
		if (e->keyuppressed && O fuel>0){
			O velocity.y-=inc;
		}
		if(e->keydownpressed && O fuel>0){
			O velocity.y+=inc;
		}
		if(e->keyleftpressed && O fuel>0){
			O velocity.x-=inc;
		}
		if(e->keyrightpressed && O fuel>0){
			O velocity.x+=inc;
		}
		if(O fuel>0 &&(e->keyrightpressed||e->keyleftpressed||e->keydownpressed||e->keyuppressed)){
			O fuel--;
			if(!O player.ison){
				O texture=O player.on;
				O player.ison=!O player.ison;
			}
			turntoangle(&(O angle),vecttoangle(O velocity.x,O velocity.y),10);
		}else{
			if(O player.ison){
				O texture=O player.off;
				O player.ison=!O player.ison;
			}
		}

}

static void actionspaceshipwaitingliftoff(Entity* enty){
	for(int i=0;i<NBENTITY;i++){
		if(ENTY.existing){
			if(amiin(ENTY.pos.x,ENTY.pos.y,enty->pos.x-(enty->w/2),enty->pos.y-(enty->h/2),enty->w,enty->h)){
				if(ENTY.player.drivenbyplayer){
					ENTY.existing=false;
					enty->player.drivenbyplayer=true;
					enty->type=OTHER;
					break;
				}
			}
		}
	}
}

static void inittabtextures(InitResult* ir){
	const char* defaultpaths [NBDEFAULTTEXTURE]={"fuel-100.bmp","fuel-80.bmp","fuel-60.bmp","fuel-40.bmp","fuel-20.bmp","fuel-0.bmp","explosion.bmp","voussortezdelarene.bmp"};
	for (int i=0;i<NBDEFAULTTEXTURE;i++){
		tabdefaulttextures [i]=loadTexture(defaultpaths[i],ir->renderer,NULL,NULL);
	}
}

static void updateui(InitResult* ir){
	if(!(ui.spaceship->existing)){
		bool o=false;
		for(int i=0;i<NBENTITY;i++){
				if(ENTY.existing && ENTY.player.drivenbyplayer){
					ui.spaceship=&ENTY;
					o=true;
				}
		}
		if(!o){
			died=true;
		}
	}
	if(!died){
		int winx;
		int winy;
		SDL_GetWindowSize(ir->window,&winx,&winy);
		while(ui.spaceship->pos.y-y<=0){
			y--;
		}
		while(ui.spaceship->pos.y-y>=winy){
			y++;
		}
		while(ui.spaceship->pos.x-x<=0){
			x--;
		}
		while(ui.spaceship->pos.x-x>=winx){
			x++;
		}
		int i=0;
		if(ui.spaceship->fuel>=100){
			i=0;
		}else{
			if((ui.spaceship->fuel>=80)){
				i=1;
			}else{
				if((ui.spaceship->fuel>=60)){
					i=2;
				}else{
					if((ui.spaceship->fuel>=40)){
						i=3;
					}else{
						if((ui.spaceship->fuel>=20)){
							i=4;
						}else{
							if((ui.spaceship->fuel>=00)){
								i=5;
							}}}}}}
		SDL_Rect rect={0,0,100,20};
		SDL_RenderCopy(ir->renderer,tabdefaulttextures[i],NULL,&rect);
		if(!ui.notifcenterempty){
			ui.notif.degrees+=ui.notif.increment;
			if(ui.notif.degrees>15){
				ui.notif.increment=-1;
			}
			if(ui.notif.degrees<-15){
				ui.notif.increment=1;
			}
			int winx;
			SDL_GetWindowSize(ir->window,&winx,NULL);
			SDL_Rect rect2={winx-200,0,200,40};
			traitetextureanimated(&(ui.notif.texture));
			SDL_RenderCopyEx(ir->renderer,ui.notif.texture.texture,NULL,&rect2,ui.notif.degrees,NULL,SDL_FLIP_NONE);
		}
	}
}

static void getevents(bool* Exit, Events* events){
	SDL_Event e;
		while(SDL_PollEvent(&e)){
			switch (e.type)
			{
				case SDL_QUIT:
					*Exit = true;
				break;
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym){
						case SDLK_ESCAPE:
							*Exit = true;
							break;
						case SDLK_UP:
							events->keyuppressed=true;
							break;
						case SDLK_DOWN:
							events->keydownpressed=true;
							break;
						case SDLK_LEFT:
							events->keyleftpressed=true;
							break;
						case SDLK_RIGHT:
							events->keyrightpressed=true;
							break;
						case SDLK_q:
							events->keyQpressed=true;
							break;
						case SDLK_d:
							events->keyDpressed=true;
							break;

					}
				break;
			}
		}
}

static void initfromfile(const char* filepath,InitResult* ir){
FILE* file=fopen(filepath,"r");
if(file==NULL){exit(0);};
char ch=' ';
char chtab[100];
int nbtextures;
int nbplanets;
int nbentities;
readlineinfile(file,chtab);
nbtextures=strtol(chtab,NULL,10);
readlineinfile(file,chtab);
nbplanets=strtol(chtab,NULL,10);
readlineinfile(file,chtab);
nbentities=strtol(chtab,NULL,10);
int* dim =(int*)malloc(sizeof(int)*nbtextures*2);
for (int o=0;o<nbtextures;o++){
	readlineinfile(file,chtab);
	tabtextures[o]=loadTexture(chtab,ir->renderer,&(dim[o*2]),&(dim[o*2+1]));
}
printf("textures loaded");
for (int o=0;o<nbentities;o++){
	TextureAnimated ta;
	Vector pos;
	Vector velocity;
	EntityType type;
	Player pl;
	double angle;
	double fuel;
	readlineinfile(file,chtab);
	ta.indexmin=strtol(chtab,NULL,10);
	readlineinfile(file,chtab);
	ta.actualindex=strtol(chtab,NULL,10);
	readlineinfile(file,chtab);
	ta.indexmax=strtol(chtab,NULL,10);
	readlineinfile(file,chtab);
	type=(EntityType)strtol(chtab,NULL,10);
	ch=fgetc(file);
	if(ch=='0'){pl.drivenbyplayer=false;}else{pl.drivenbyplayer=true;}
	fgetc(file);
	readlineinfile(file,chtab);
	pl.force=strtol(chtab,NULL,10);
	if(pl.drivenbyplayer){
		readlineinfile(file,chtab);
		pl.on.indexmin=strtol(chtab,NULL,10);
		readlineinfile(file,chtab);
		pl.on.actualindex=strtol(chtab,NULL,10);
		readlineinfile(file,chtab);
		pl.on.indexmax=strtol(chtab,NULL,10);
		readlineinfile(file,chtab);
		pl.off.indexmin=strtol(chtab,NULL,10);
		readlineinfile(file,chtab);
		pl.off.actualindex=strtol(chtab,NULL,10);
		readlineinfile(file,chtab);
		pl.off.indexmax=strtol(chtab,NULL,10);
	}
	readlineinfile(file,chtab);
	angle=strtol(chtab,NULL,10);
	readlineinfile(file,chtab);
	pos.x=strtol(chtab,NULL,10);
	readlineinfile(file,chtab);
	pos.y=strtol(chtab,NULL,10);
	readlineinfile(file,chtab);
	velocity.x=strtol(chtab,NULL,10);
	readlineinfile(file,chtab);
	velocity.y=strtol(chtab,NULL,10);
	readlineinfile(file,chtab);
	fuel=strtol(chtab,NULL,10);
	createentity({type,pl,ta,{6,6,6,NULL,true},dim[ta.actualindex*2],dim[ta.actualindex*2+1],angle,pos,velocity,0,0,fuel,true}); // @suppress("Invalid arguments")
}
printf("entities loaded");
for (int o=0;o<nbplanets;o++){
	TextureAnimated ta;
	Vector pos;
	int mass;
	bool fixed;
	double maxspeedbeforeexplosion;
	readlineinfile(file,chtab);
	printf(chtab);
	ta.indexmin=strtol(chtab,NULL,10);
	readlineinfile(file,chtab);
	ta.actualindex=strtol(chtab,NULL,10);
	readlineinfile(file,chtab);
	ta.indexmax=strtol(chtab,NULL,10);
	readlineinfile(file,chtab);
	pos.x=strtol(chtab,NULL,10);
	readlineinfile(file,chtab);
	pos.y=strtol(chtab,NULL,10);
	readlineinfile(file,chtab);
	maxspeedbeforeexplosion=strtol(chtab,NULL,10);
	readlineinfile(file,chtab);
	mass=strtol(chtab,NULL,10);
	ch=fgetc(file);
	if(ch=='0'){fixed=false;}else{fixed=true;}
	fgetc(file);
	readlineinfile(file,chtab);
	Entity* enty=createplanet({ta,pos,SDL_LoadBMP(chtab),(int)dim[ta.actualindex*2],(int)dim[ta.actualindex*2+1],mass,maxspeedbeforeexplosion,fixed,true}); // @suppress("Invalid arguments")
	if(!fixed){
		readlineinfile(file,chtab);
		enty->velocity.x=strtol(chtab,NULL,10);
		readlineinfile(file,chtab);
		enty->velocity.y=strtol(chtab,NULL,10);
	}
}
printf("planets loaded");
readlineinfile(file,chtab);
int index=strtol(chtab,NULL,10);
printf("%d",index);
ui.spaceship=&(tabentity[index]);
ch=fgetc(file);
if(ch=='0'){leveltype=STAYINARENALEVEL;}else{leveltype=CAPTURETHEWINLEVEL;}
fgetc(file);
arenaw=dim[0];
arenah=dim[1];
}

static void tick(InitResult* ir,Events e,int w, int h,bool* Exit){
	if(amiin(ui.spaceship->pos.x,ui.spaceship->pos.y,0,0,w,h)){ui.notifcenterempty=true;}else{
		if(ui.notifcenterempty || (ui.notif.type!=OUTOFARENANOTIF)){
			TextureAnimated tex={7,7,7,NULL,true};
			ui.notif.texture=tex;
			ui.notifcenterempty=false;
			ui.notif.type=OUTOFARENANOTIF;
		}
	}
	if(tickcounter>9000 && leveltype==STAYINARENALEVEL && (died==false)){
		if(amiin(ui.spaceship->pos.x,ui.spaceship->pos.y,0,0,w,h)){
			winanim(ir);
			(*Exit)=true;
			return;
		}
	}
	if(e.keyDpressed && (delay>0)){
		delay--;
	}
	if(e.keyQpressed){
		delay++;
	}
	tickcounter++;
	SDL_Delay(delay);
}

void loop(InitResult* ir){
	for(int i=0;i<NBENTITY;i++){
		tabentity[i]=*(new Entity);
	}
	for(int i=0;i<NBTEXTURE;i++){
		tabtextures[i]=NULL;
	}
	for(int i=0;i<NBPLANET;i++){
		tabplanets[i]=*(new Planet);
	}
	for(int i=0;i<NBENTITY;i++){
		tabexplosion[i]=*(new Explosion);
	}
	ui=*(new Ui);
initfromfile("lv1.txt",ir);
inittabtextures(ir);
SDL_SetWindowFullscreen(ir->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
bool Exit=false;
died=false;
win=false;
x=0;
y=0;
delay=100;
tickcounter=0;
while(!Exit){
	SDL_SetRenderDrawColor(ir->renderer, 0, 0, 0, 255);
	SDL_RenderClear(ir->renderer);
	Events e;
	getevents( &Exit, &e);
	int winx;
	int winy;
	SDL_GetWindowSize(ir->window,&winx,&winy);
	SDL_Texture* map=tabtextures[0];
	SDL_Rect src{x-(winx/2),y-(winy/2), winx, winy};
	SDL_Rect dist{ 0, 0, winx, winy};
	SDL_RenderCopy(ir->renderer,map,&src,&dist);
	for(int i=0;i<NBENTITY;i++){
		if(ENTY.existing){
			if(ENTY.player.drivenbyplayer){
				actionspaceship(&ENTY,ir,&e,ENTY.player.force);
			}
			if(ENTY.type==SPACESHIPWAITINGLIFTOFF){
				actionspaceshipwaitingliftoff(&ENTY);
			}
			if(ENTY.type==WIN){
				actionwin(ir,&ENTY,&Exit);
			}
			if(ENTY.type==PLANET){
				actionplanet(&ENTY);
			}
		traitentity(&ENTY,ir,x,y);
		}

	}
	for(int i=0;i<NBPLANET;i++){
		if(tabplanets[i].existing){
			traiteplanet(&(tabplanets[i]),i,ir,x,y);
		}
	}
	for(int i=0;i<NBENTITY;i++){
		if(tabexplosion[i].existing){
			traiteexplosion(&(tabexplosion[i]),ir);
		}
	}
	updateui(ir);
	SDL_RenderPresent(ir->renderer);
	tick(ir,e,arenaw,arenah,&Exit);
}
SDL_SetWindowFullscreen(ir->window, 0);
}



