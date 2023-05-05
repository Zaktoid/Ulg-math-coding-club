#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>


typedef struct {double Rz;double Iz;}complex_num;//definition d'un nombre complexe
typedef struct 
{
    float inf;
    float sup;
    float actu;
    float length;
    int X;
    int Y;
}slider;
void Upgrage_slider(slider* slider,float incr)
{
    if(slider->actu+incr>slider->inf && slider->actu+incr<slider->sup)
        slider->actu+=incr;
}
void Draw_slider(slider slider,SDL_Renderer * rendu)
{
    SDL_Rect line={slider.X,slider.Y,slider.length,3};
    SDL_Rect dot={slider.X+((slider.actu-slider.inf)/slider.sup)*(slider.length),slider.Y,6,10};
    SDL_SetRenderDrawColor(rendu,0,0,100,255);
    SDL_RenderDrawRect(rendu,&line);
    SDL_SetRenderDrawColor(rendu,0,0,0,255);
    SDL_RenderDrawRect(rendu,&dot);
    
}

complex_num prod(complex_num a,complex_num b)//definition du produit
{
    complex_num pr;
    pr.Rz=(a.Rz)*(b.Rz)-(a.Iz)*(b.Iz);
    pr.Iz=(b.Iz)*(a.Rz)+(b.Rz)*(a.Iz);
    return pr;
}
complex_num sum(complex_num a, complex_num b)//definition de la somme
{
    complex_num sum={a.Rz+b.Rz,b.Iz+a.Iz};
    return sum;
}
complex_num Pow(complex_num z,int n)
{
    complex_num out={1,0};
    for(int k=0;k<n;k++)
    {
        out=prod(out,z);
    }
    return out;
}
complex_num Inverse(complex_num z)
{
    complex_num out = {(z.Rz)/(z.Rz*z.Rz+z.Iz*z.Iz),-1*((z.Iz)/(z.Rz*z.Rz+z.Iz*z.Iz))};
    return out;
}
complex_num opp(complex_num z)
{
    complex_num out;
    out.Iz=-z.Iz;
    out.Rz=-z.Rz;
    return out;
}
double module(complex_num a)//definition du module
{    
    return sqrt((a.Rz)*(a.Rz)+(a.Iz)*(a.Iz));
}

complex_num Exp_c(complex_num z,int iter)
{
    complex_num out={1,0};
    complex_num div={1,0};
    for(int k=1;k<iter;k++)
    {   div.Rz/=k;
        out=sum(out,prod(div,Pow(z,k)));
    }
    return out;
} 
typedef struct 
{
    complex_num* coef;
    int deg;
}Poly;
Poly* Init_Poly(int deg)
{
    Poly* out=malloc(sizeof(Poly));
    if(!out)
    {
        printf("Error init poly");
        return NULL;
    }
    out->deg=deg;
    out->coef=malloc(sizeof(complex_num)*(deg+1));
    if(!out->coef)
    {
        free(out);
        printf("Error init poly");
        return NULL;
    }
    for(int k=0;k<=deg;k++)
    {
        out->coef[k].Iz=0;
        out->coef[k].Rz=0;
    }
    return out;
}

complex_num Poly_ev(Poly P,complex_num z)
{
    complex_num out={0,0};
    for(int k=0;k<=P.deg;k++)
    {
        out=sum(out,prod(P.coef[k],Pow(z,k)));
    }
    return out;
}
