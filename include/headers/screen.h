#include "rect.h"
#include <SDL.h>
#include "auto.h"
#include <SDL2_gfxPrimitives.h>
#include <iostream>

class Screen {
    private:
        int w, h;
        double dt_ms;
    public:
        //Konstruktor
        Screen(int w, int h, double dt_ms) {
            this->w = w;
            this->h = h;
            this->dt_ms = dt_ms;
        }
        //Tagfüggvények
        void DrawSine(SDL_Renderer *renderer, double amp, double freq, int y0, int r, int g, int b);
        void sdl_init(SDL_Window **pwindow, SDL_Renderer **prenderer);
        void abrazol(Auto auto1, int w_r, int h_r, double steerang);
        void Redraw(SDL_Renderer *renderer, SDL_Window *window, Auto &auto1, rect rect, int w_r, int h_r, double angle, double delta, int *rasegites, const double steerang);
};