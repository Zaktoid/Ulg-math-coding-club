#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "Mathutils.h"
void SDL_Exitwitherror(const char *message)
{
	SDL_Log("Erreur:%s >%s \n",message,SDL_GetError());
	exit(EXIT_FAILURE);


}
int main()
{

    int NeedTodraw=1;
    float coefR=10;
    float coefG=10;
    float coefV=10;
    float vert=0;
    float horiz=0;
    float zoom=1;
    int hauteur=400;
    int deg=3;
    int* pdeg=&deg;
    int tr=0;
    complex_num C[deg];
    float a=0;
    for (int k=0;k<deg;k++)
    {
        C[k].Rz=cos(2*3.14*k/deg);
        C[k].Iz=sin(2*3.14*k/deg);
    }
    complex_num PolyR(complex_num z)
    {
        complex_num out={0,1};
        for(int k=0;k<*pdeg;k++)
        {
            out=prod(out,sum(z,opp(C[k])));
        }
        return out;
    }
    SDL_Window *wdw;
	SDL_Renderer *rendu;
	if(SDL_Init(SDL_INIT_VIDEO))
		SDL_Exitwitherror("failed init");
	wdw = SDL_CreateWindow("Newton Fractal",SDL_WINDOWPOS_CENTERED,0,hauteur,hauteur,0);
    rendu = SDL_CreateRenderer(wdw,-1,SDL_RENDERER_SOFTWARE);			
	if(!wdw)
		SDL_Exitwitherror("window creation failed");
	if(!rendu)
		SDL_Exitwitherror("renderer failed");
	SDL_bool Launched= SDL_TRUE;
    while (Launched)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                Launched= SDL_FALSE;
                break;
            break;
            case SDL_KEYDOWN:
                NeedTodraw=1;
                switch(event.key.keysym.sym)
                {   case(SDLK_s):
                        coefR*=1.1;
                        break;
                    case(SDLK_q):
                        coefR=coefR/1.1;
                        break;
                    case(SDLK_w):
                        coefG=coefG/1.1;
                        break;
                    case(SDLK_x):
                        coefG=coefG*1.1;
                        break;
                    case(SDLK_d):
                        coefV=coefV/1.1;
                        break;
                    case(SDLK_f):
                        coefV*=1.1;
                        break;
                    case(SDLK_o):
                        C[0].Rz-=0.1;
                        break;
                    case(SDLK_p):
                        C[0].Rz+=0.1;
                        break;
                    case(SDLK_l):
                        C[0].Iz-=0.1;
                        break;
                    case(SDLK_m):
                        C[0].Iz+=0.1;
                        break;
                    case(SDLK_v):
                        tr+=1;
                        break;
                    case(SDLK_c):
                        tr-=1;
                        break;
                case SDLK_RIGHT:
                    horiz+=10;
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
                case SDLK_z:
                    zoom*=1.1;
                    break;
                case SDLK_a:
                    zoom/=1.1;
                    break;
                case SDLK_h:
                    a+=0.1;
                    break;
                case SDLK_j:
                    a-=0.1;
                    break;
                case SDLK_ESCAPE:
                    Launched=SDL_FALSE;
                    break;
                default:
                    break;}
            default:
                break;
            
            }
            
        }
        complex_num cis={cos(a),sin(a)};
        if(NeedTodraw)
        {
            for(int i=0;i<hauteur;i+=1)
            for(int j=0;j<hauteur;j+=1)
            {
                complex_num z={zoom*((i-hauteur/2)+horiz)/hauteur,zoom*((j-hauteur/2)+vert)/hauteur};
                z=prod(z,cis);
                complex_num h={0.0001,0.0001};
                complex_num u=Newton(z,PolyR,tr,h);
                SDL_SetRenderDrawColor(rendu,55*tanh(u.Rz*coefG),105*tanh(u.Iz*coefV),255*tanh(module(u)*coefR),255);
                SDL_RenderDrawPoint(rendu,i,j);

            }
        SDL_RenderPresent(rendu);

        NeedTodraw=0;}
        
    }
    SDL_DestroyRenderer(rendu);
	SDL_DestroyWindow(wdw);
	SDL_Quit();
	return EXIT_SUCCESS;
}