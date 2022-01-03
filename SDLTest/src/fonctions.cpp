#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
SDL_Texture* loadTexture( std::string path , SDL_Renderer* renderer,int* w,int* h)
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    if(w!=NULL){
    	SDL_QueryTexture(newTexture, nullptr, nullptr, w, h);
    }
    return newTexture;
}
void turntoangle(double* angle, double otherangle, double increment){
	double dista=0;
	double distb=0;
	while(otherangle>=360){
		otherangle-=360;
	}
	while(otherangle<0){
		otherangle+=360;
	}
	if(*angle<otherangle){
		dista= otherangle- *angle;
		distb= 360-otherangle+ *angle;
	}
	if(*angle>otherangle){
		dista= 360-*angle+otherangle;
		distb= *angle-otherangle;
	}
	if(dista<distb){
		*angle+=increment;
	}else{
		*angle-=increment;
	}
	while(*angle>=360){
		*angle-=360;
	}
	while(*angle<0){
		*angle+=360;
	}
}
double vecttoangle(int vectorx,int vectory){
	int a=vectorx;
	int b=vectory;
	if(b<0){
		return          (asin(a/sqrt(a*a+b*b))) * (180.0/3.141592653589793238463);
	}else{
		if(a>0){
			return  180-(asin(a/sqrt(a*a+b*b))) * (180.0/3.141592653589793238463);
		}else{
			return -180-(asin(a/sqrt(a*a+b*b))) * (180.0/3.141592653589793238463);
		}
	}
}
double distint(double fromx, double fromy, double tox, double toy){
	double x=tox-fromx;
	double y=toy-fromy;
	return sqrt(x*x+y*y);
}
void shortervector(double lengh, double posx, double posy, double* shorterx, double* shortery){
	double c=distint(0,0,posx,posy);
	double B=c-lengh;
	*shortery=posy*(lengh/B);
	if(posx>0){
		*shorterx=sqrt((lengh*lengh)-((*shortery)*(*shortery)));
	}else{
		*shorterx=(-(sqrt((lengh*lengh)-((*shortery)*(*shortery)))));
	}
	if(*shorterx!=*shorterx){
		*shorterx=0;
	}
}
Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

switch (bpp)
{
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN){
            return p[0] << 16 | p[1] << 8 | p[2];
        }else{
            return p[0] | p[1] << 8 | p[2] << 16;}
            break;

        case 4:
            return *(Uint32 *)p;
            break;

        default:
            return 0;       /* shouldn't happen, but avoids warnings */
      }
}
bool amiin(double mex, double mey, double itx, double ity, double w, double h){
	return (((mex>itx) && (mex<(itx+w))) && ((mey>ity) && (mey<(ity+h))));
}
void readlineinfile(FILE* file,char* chtab){
	int i=0;
	char ch;
	do{
		ch=fgetc(file);
		if(ch!='\n'){chtab[i]=ch;}else{chtab[i]='\0';}
		i++;
	}while(ch!='\n');
}
