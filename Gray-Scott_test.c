#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
void SDL_Exitwitherror(const char *message)
{
	SDL_Log("Erreur:%s >%s \n",message,SDL_GetError());
	exit(EXIT_FAILURE);

}
float CountN(float *Cells,int x , int y,int taille)
{
	float s=0;
	for(int i=-1;i<=1;i++)
	{
		for(int j=-1;j<=1;j++)
		{
            if(i!=0 || j!=0)
			s+=Cells[(j+y)*taille+x+i];
		}
	}
	return s;
}


int main()
{
    /********variables principales*****/
    unsigned B=1;
    int hauteur=400;
    int taille=100;
    float *cases=malloc(2*taille*taille*sizeof(float));
    float *cases_s=malloc(2*taille*taille*sizeof(float));
    int pause=0;
    float prop=1;
    float apporov=0.01;
    float deg=0;
    for (int i=0;i<2*taille*taille;i++ )
	{ 
        cases[i]=0;
        cases_s[i]=0;
    }
    SDL_Window *wdw;
	SDL_Renderer *rendu;
	SDL_Texture *text;
	if(SDL_Init(SDL_INIT_VIDEO))
		SDL_Exitwitherror("failed init");
	wdw = SDL_CreateWindow("Gray Scott",SDL_WINDOWPOS_CENTERED,0,hauteur,hauteur,0);			
	if(!wdw)
		SDL_Exitwitherror("window creation failed");
	rendu = SDL_CreateRenderer(wdw,-1,SDL_RENDERER_SOFTWARE);
	if(!rendu)
		SDL_Exitwitherror("renderer failed");
	SDL_bool Launched= SDL_TRUE;
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
            case SDL_MOUSEBUTTONDOWN:
                int x=event.button.x;
                int y=event.button.y;
                for(int i=-4;i<=4;i++)
                {
                    for(int l=-4;l<=4;l++)
                    {cases[taille*((y+i)*taille/hauteur)+(taille*(x+l)/hauteur)]+=1*B;}
                }                
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case(SDLK_p):
                        pause=1-pause;
                        break;
                    case(SDLK_d):
                        if(prop-0.01>0)
                        prop-=0.01;
                        break;
                    case(SDLK_e):
                        prop+=0.01;
                        break;
                    case(SDLK_a):
                        B=1-B;
                    default:
                        break;
                }
			default:
				break;
			}
        }
    if(!pause)
	{
		for(int l=1;l<(taille-1);l++)
		{
			for(int k=1;k<(taille-1);k++)
			{
                //mecanisme de diffusion
                cases_s[l*taille+k]=(cases[l*taille+k]*(1-prop-deg)
                +CountN(cases,k,l,taille)*prop/8);
            }
        }
    }

		    
	    
		for(int l=0;l<(taille);l++)
			{
				for (int k=0;k<taille;k++)
				{
					if(!pause)
                        cases[l*taille+k]=cases_s[l*taille +k];
					SDL_Rect rectangle={k*(hauteur/taille),(hauteur/taille)*(l),hauteur/taille,hauteur/taille};
                    if(cases[l*taille+k])
					    {
                        SDL_SetRenderDrawColor(rendu,tanh(cases[l*taille+k])*254,0,0,255);
					    SDL_RenderFillRect(rendu, &rectangle);
                        }
					
					
				}
			}
            SDL_RenderPresent(rendu);
            SDL_SetRenderDrawColor(rendu,0,0,0,255);
            SDL_RenderClear(rendu);
    }	
		

    





    /************désallocation !************/
    free(cases);
    free(cases_s);
    SDL_DestroyRenderer(rendu);
	SDL_DestroyWindow(wdw);
	SDL_Quit();
	return EXIT_SUCCESS;

}