#include <iostream>
#include <SDL2/SDL.h>
#include"fonctions.h"
#ifndef _LOOP_H_
#define _LOOP_H_
enum EntityType{
	OTHER,
	PLANET,
	SPACESHIPWAITINGLIFTOFF,
	WIN,
	LEVEL,
};
typedef struct TextureAnimated{
	int indexmin;
	int actualindex;
	int indexmax;
	SDL_Texture* texture;
	bool isdefaulttable=false;
}TextureAnimated;
typedef struct Vector{
	double x=0;
	double y=0;
}Vector;
typedef struct Planet{
	TextureAnimated texture;
	Vector pos;
	SDL_Surface* groundsurfacepath;
	int w;
	int h;
	int mass;
	double maxspeedbeforeexplosion;
	bool fix=true;
	bool existing=false;
}Planet;
typedef struct Player{
	bool drivenbyplayer;
	double force;
	TextureAnimated on;
	TextureAnimated off;
	bool ison=false;
}Player;
typedef struct Entity{
	EntityType type;
	Player player;
	TextureAnimated texture;
	TextureAnimated explosiontexture={6,6,6,NULL,true};
	int w;
	int h;
	double angle;
	Vector pos;
	Vector velocity;
	int indexactualplanet;
	int planetindex;
	double fuel;
	bool existing=false;

}Entity;
typedef struct Events{
	bool keyuppressed=false;
	bool keydownpressed=false;
	bool keyleftpressed=false;
	bool keyrightpressed=false;
	bool keyQpressed=false;
	bool keyDpressed=false;
}Events;
enum NotifType{
	OUTOFARENANOTIF,
};
typedef struct Notif{
	TextureAnimated texture;
	int degrees;
	int increment=1;
	NotifType type;
}Notif;
typedef struct Ui{
	Entity* spaceship;
	Notif notif;
	bool notifcenterempty=true;
}Ui;
enum LevelType{
	STAYINARENALEVEL,
	CAPTURETHEWINLEVEL,
};
typedef struct Explosion{
	TextureAnimated texture;
	double size = 0.1;
	bool incrasesize=true;
	Vector pos;
	bool existing=false;
}Explosion;
void loop(InitResult* ir);
void loop(InitResult* ir,char* file);
#endif
