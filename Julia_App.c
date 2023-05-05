#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <math.h>
#include "Mathutils.h"

complex_num c={-1.417022285618, 0.0099534};//Variable global pour la "nature" de la fractale

complex_num Mandel(complex_num a)//Application du type z²+c
{
    return sum(prod(a,a),c);
}
int Test_Diverg(int tres,complex_num a,complex_num (*application)(complex_num),double toler)
{
complex_num iteration=a;
for (double i=0;i<tres;i++)
{
    iteration=application(iteration);//"application" est une fonction de C->C passée en argument
    if(module(iteration)>toler*toler)//on teste par rapport au carré
    {
        return i;
    }
}
return 0;
}
void SDL_Exitwitherror(const char *message)
{
	SDL_Log("Erreur:%s >%s \n",message,SDL_GetError());
	exit(EXIT_FAILURE);
}



int main(int argc , char **argv)
{
	int NeedToDraw=1;
	float z=1;//facteur de zoom
	float vert=0;
	float horiz=0;
	int coefR=1;
	int coefG=1;
	int coefB=1;
	int hauteur=1500;
	int largeur=800;
	SDL_Window *wdw;
	SDL_Renderer *rdrd;

    /**********intialisation d'élements SDL************/
	if(SDL_Init(SDL_INIT_VIDEO))
		SDL_Exitwitherror("Echec init");
	wdw = SDL_CreateWindow("Julia Explorer !",SDL_WINDOWPOS_CENTERED,0,hauteur,largeur,0);			
	if(!wdw)
		SDL_Exitwitherror("Creation fenêtre échouée");
	rdrd = SDL_CreateRenderer(wdw,-1,SDL_RENDERER_SOFTWARE);
	if(!rdrd)
		SDL_Exitwitherror("Echec Rendu");
	SDL_bool Launched= SDL_TRUE;


    /************Boucle principale***********************/
	while (Launched)
	{

		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				Launched= SDL_FALSE;
				break;
			case SDL_KEYDOWN:
				NeedToDraw=1;//On calcule les valeurs seulement si elles ont été modifiées
				switch (event.key.keysym.sym)
				{
				
				case SDLK_RIGHT:
					horiz+=10;
					break;
				case SDLK_SPACE:
					z=1;
					break;
				case SDLK_LEFT:
					horiz-=10;
					break;
				case SDLK_UP:
					vert-=10;
					break;
				case SDLK_DOWN:
					vert+=10;
					break;
				case SDLK_r:
					coefR+=1;
					break;
				case SDLK_g:
					coefG+=1;
					break;
				case SDLK_b:
					coefB+=1;
					break;
				case SDLK_v:
					coefB-=1;
					break;
				case SDLK_e:
					coefR-=1;
					break;
				case SDLK_f:
					coefG-=1;
					break;
				case SDLK_k:
					c.Rz+=(0.001)/z;
					break;
				case SDLK_l:
					c.Iz+=(0.001)/z;
					break;
				case SDLK_i:
					c.Rz-=(0.001)/z;
					break;
				case SDLK_o:
					c.Rz-=(0.001)/z;
					break;
				case SDLK_a:
					z*=1.1;
					break;
				case SDLK_q:
					{z/=1.1;}
					break;
				default:
					break;
				
				}
			default:
				break;
		}}



		if(NeedToDraw)
		{
			for(int k=0;k<hauteur;k+=2)
			{
				for(int l=0;l<largeur;l+=2)
				{
					complex_num a={((double)k-(hauteur/2)+horiz)/(z*(hauteur/4)),
					((double)l-(hauteur/2)+vert)/(z*(hauteur/4))};
					int s=Test_Diverg(5*z,a,Mandel,10);
					if(SDL_SetRenderDrawColor(rdrd,(coefR)*s,(coefG)*s,(coefB)*s,255))
					SDL_Exitwitherror("Echec pinceau");
					SDL_RenderDrawPoint(rdrd,k,l);

				}
			}

		    SDL_RenderPresent(rdrd);
		    NeedToDraw=0;
		}
	}
	
/*************************************/
	SDL_DestroyRenderer(rdrd);
	SDL_DestroyWindow(wdw);
	SDL_Quit();
	return EXIT_SUCCESS;
}

