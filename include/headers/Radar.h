#include <iostream>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <vector>
#include "auto.h"

struct Coords
{
    int x;
    int y;
};

typedef std::vector<Coords> savadat;

class radar {
    private:
        int w;
        int h;
        SDL_Surface *surface;
    public:
        //Konstruktor
        radar(int w=0, int h=0, SDL_Surface *surface=NULL) {
            this->w = w;
            this->h = h;
            this->surface = surface;
        }
        //Radar
        ~radar() {
            SDL_FreeSurface(surface);
        }
        //Tagfüggvények
        bool isWhite(int x, int y); // háttér fekete, autó kék, akadály fehér
        bool detectObs(int x0, int y0, double angle, int hatotav); // (x0, y0) környékén akadályokat keres x szögben egy egyenes mentén (pl. 90° -> lefelé)
        bool detectPoints(int x0, int y0, double rad, Coords &adat, double max);
        savadat savDetect(int x0, int y0, int w, int h, int w_r, int h_r, int angle);
        double deltaszam(int x0, int y0, int theta0, savadat savadatok, double &angle_err, double &dist_err, Auto &auto1);
};