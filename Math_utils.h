#ifndef MATHUTILS_H
#define MATHUTILS_H
typedef struct {double Rz;double Iz;}complex_num;
typedef struct 
{
    complex_num* coef;
    int deg;
}Poly;
typedef struct 
{
    float inf;
    float sup;
    float actu;
    float length;
    int X;
    int Y;
}slider;
void Upgrage_slider(slider* slider,float incr);

void Draw_slider(slider slider,SDL_Renderer * rendu);


complex_num prod(complex_num a,complex_num b);//definition du produit

complex_num sum(complex_num a, complex_num b);//definition de la somme

complex_num Pow(complex_num z,int n);

complex_num Inverse(complex_num z);

complex_num opp(complex_num z);

double module(complex_num a);//definition du module

complex_num Exp_c(complex_num z,int iter);

Poly* Init_Poly(int deg);

complex_num Poly_ev(Poly P,complex_num z);
#endif