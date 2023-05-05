#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>


typedef struct
{
    float Temp;
    float C_bois;
} Point;

void SDL_Exitwitherror(const char *message)
{
    SDL_Log("Erreur:%s >%s \n", message, SDL_GetError());
    exit(EXIT_FAILURE);
}

void InitArray(Point *Array, Point *Array_n, int taille)
{
    for (int k = 0; k < taille*taille; k++)
    {
        int s = rand();
        if (s%12 == 0)
            Array[k].C_bois = abs(s)/100;
        Array[k].Temp = 20;
        Array_n[k].C_bois = Array[k].C_bois;
        Array_n[k].Temp = Array[k].Temp;
    }
}

void Diffus(Point *Array_out, Point *Array_in, int x, int y, int taille, float D)
{
    Array_out[x*taille+y].Temp = Array_in[x*taille+y].Temp*(1-D);
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            if (j != 0 || i != 0)
            {
                Array_out[x*taille+y].Temp += Array_in[(x+i)*taille+y+j].Temp*D/8;
            }

        }
    }
}

int main()
{
    srand(time(0));
    int hauteur = 700;
    int taille = 350;
    Point *Grille = malloc(taille*taille*sizeof(Point));
    Point *Grille_n = malloc(taille*taille*sizeof(Point));
    InitArray(Grille, Grille_n, taille);
    /***********************/
    SDL_Window *wdw;
    SDL_Renderer *rendu;
    SDL_Texture *text;
    SDL_Surface *surface;
    if (SDL_Init(SDL_INIT_VIDEO))
        SDL_Exitwitherror("failed init");
    wdw = SDL_CreateWindow("Nom fenêtre", SDL_WINDOWPOS_CENTERED, 0, hauteur, hauteur, 0);
    if (!wdw)
        SDL_Exitwitherror("window creation failed");
    rendu = SDL_CreateRenderer(wdw, -1, SDL_RENDERER_SOFTWARE);
    if (!rendu)
        SDL_Exitwitherror("renderer failed");
    SDL_bool Launched = SDL_TRUE;
    int pause = 1;
    float k_c = 0.7;
    float h = 0.2;
    float C = 1;
    /************************/
    while (Launched)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    Launched = SDL_FALSE;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case (SDLK_p):
                            pause = 1-pause;
                        default:
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    int x = event.button.x;
                    int y = event.button.y;
                    for (int i = -4; i <= 4; i++)
                    {
                        {
                            for (int l = -4; l <= 4; l++)
                                Grille[taille*((y+i)*taille/hauteur)+(taille*(x+l)/hauteur)].Temp += 40;
                        }
                    }
                    break;

                default:
                    break;
            }
        }
        if (!pause)
        {
            for (int l = 1; l < (taille-1); l++)
            {
                for (int k = 1; k < (taille-1); k++)
                {
                    /******Combustion*******/
                    if (Grille[l*taille+k].Temp >= 30)
                    {
                        Grille_n[l*taille+k].C_bois = (1-k_c)*(Grille[l*taille+k].C_bois);
                        Grille_n[l*taille+k].Temp = k_c*(Grille[l*taille+k].C_bois);
                    }
                    else
                        /*********Diffusion thermique*********/
                        Diffus(Grille_n, Grille, k, l, taille, 0.8);

                }
            }
        }
        for (int l = 0; l < (taille); l++)
        {
            for (int k = 0; k < taille; k++)
            {
                if (!pause)
                {
                    Grille[l*taille+k].C_bois = Grille_n[l*taille+k].C_bois;
                    Grille[l*taille+k].Temp = Grille_n[l*taille+k].Temp;
                }
                SDL_Rect rectangle = {k*(hauteur/taille), (hauteur/taille)*(l), hauteur/taille, hauteur/taille};
                {
                    SDL_SetRenderDrawColor(rendu, Grille[l*taille+k].Temp, Grille[l*taille+k].C_bois, 100, 255);
                    SDL_RenderFillRect(rendu, &rectangle);
                }
            }
        }
        SDL_RenderPresent(rendu);

    }
    /***********************/
    free(Grille);
    free(Grille_n);
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(wdw);
    SDL_Quit();
    return EXIT_SUCCESS;
}