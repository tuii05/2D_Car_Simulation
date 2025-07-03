#include <iostream>
#include "auto.h"
#include "Radar.h"
#include <vector>



struct Akadalyok
{
    int x;
    int y;
};

typedef std::vector<Akadalyok> barriers;

class Assists
{
private:
    int x;
    int y;
    int w;
    int h;
    int w_r;
    int h_r;
    double steerang;
public:
    //Konstruktor
    Assists(int x = 0, int y = 0, int w = 0, int h = 0, int w_r=0, int h_r=0, double steerang=0) : x(x), y(y), w(w), h(h), w_r(w_r), h_r(h_r), steerang(steerang) {}
    //Tagfüggvények
    void savtartas(Auto &auto1, SDL_Renderer *renderer, SDL_Window *window, int *rasegites);
    void tavolsagtartas(Auto &auto1, SDL_Renderer *renderer, SDL_Window *window, int *rasegites);
};