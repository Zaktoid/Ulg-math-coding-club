#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

typedef struct
{
    float *v;
    int *pos;
} mobile;

/**************************************************************/
void SDL_Exitwitherror(const char *message)
{
    SDL_Log("Erreur:%s >%s \n", message, SDL_GetError());
    exit(EXIT_FAILURE);

}

void Collide(mobile **particules, int taille)
{
    for (int i = 0; i < taille; i++)
    {
        for (int k = 0; k < taille; k++)
        {
            if (particules[i]->pos[0] == particules[k]->pos[0])
            {
                particules[i]->v[0] *= (-1);
                particules[k]->v[0] *= (-1);
            }
            if (particules[i]->pos[1] == particules[k]->pos[1])
            {
                particules[i]->v[1] *= (-1);
                particules[k]->v[1] *= (-1);
            }
        }
    }
}

mobile *Initmob(int dim)
{
    mobile *out = malloc(sizeof(mobile));
    if (!out)
        return NULL;
    out->pos = malloc(dim*sizeof(int));
    if (!(out->pos))
    {
        free(out);
        return NULL;
    }
    for (int k = 0; k < dim; k++)
        out->pos[k] = 0;
    out->v = malloc(dim*sizeof(float));
    if (!(out->v))
    {
        free(out->pos);
        free(out);
        return NULL;
    }
    for (int k = 0; k < dim; k++)
        out->v[k] = 0;
    return out;
}

void Destroy(mobile *mobile)
{
    free(mobile->pos);
    free(mobile->v);
    free(mobile);
}

int main()
{
    srand(time(NULL));
    int k = 0;
    int hauteur = 400;
    int resolution = 200;
    SDL_Window *wdw;
    SDL_Renderer *rendu;
    SDL_Texture *text;
    if (SDL_Init(SDL_INIT_VIDEO))
        SDL_Exitwitherror("failed init");
    wdw = SDL_CreateWindow("Gray Scott", SDL_WINDOWPOS_CENTERED, 0, hauteur, hauteur, 0);
    if (!wdw)
        SDL_Exitwitherror("window creation failed");
    rendu = SDL_CreateRenderer(wdw, -1, SDL_RENDERER_SOFTWARE);
    if (!rendu)
        SDL_Exitwitherror("renderer failed");
    SDL_bool Launched = SDL_TRUE;
    char *locations = malloc(resolution*resolution*sizeof(char));
    for (int k = 0; k < resolution*resolution; k++)
    {
        locations[k] = 0;
    }
    mobile **particules = malloc(10*sizeof(mobile *));
    for (int k = 0; k < 10; k++)
    {
        particules[k] = Initmob(2);
        particules[k]->pos[0] = k*k;
        particules[k]->pos[1] = k;
        particules[k]->v[0] = particules[k]->v[1] = 1;
        locations[particules[k]->pos[0]*resolution+particules[k]->pos[1]] = 1;
    }


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
                default:
                    break;

            }
        }
        for (int i = 0; i < 10; i++)
        {
            SDL_Rect rectangle = {(hauteur/resolution)*(particules[i]->pos[1]),
                                  (hauteur/resolution)*(particules[i]->pos[0]), hauteur/resolution, hauteur/resolution};
            SDL_SetRenderDrawColor(rendu, 50*i, 100, i*100, 255);
            SDL_RenderFillRect(rendu, &rectangle);
            locations[resolution*particules[i]->pos[0]+particules[i]->pos[1]] = 0;
            particules[i]->pos[0] += (particules[i]->v[0]);
            particules[i]->pos[1] += (particules[i]->v[1]);
            if (locations[resolution*particules[i]->pos[0]+particules[i]->pos[1]] == 1)
            {
                particules[i]->v[0] *= (-1);
                particules[i]->v[1] *= (-1);

            }
            else
            {
                locations[resolution*particules[i]->pos[0]+particules[i]->pos[0]] = 1;
            }
            if (particules[i]->pos[0] >= resolution || particules[i]->pos[0] <= 0)
                particules[i]->v[0] *= (-1);
            if (particules[i]->pos[1] >= resolution || particules[i]->pos[1] <= 0)
                particules[i]->v[1] *= (-1);
            SDL_Delay(5);


        }

        SDL_RenderPresent(rendu);
        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
        SDL_RenderClear(rendu);

    }


    /**************************DESTROYING*********************************/
    for (int k = 0; k < 10; k++)
        Destroy(particules[k]);
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(wdw);
    SDL_Quit();
    return EXIT_SUCCESS;


}
