#include <SDL2_gfxPrimitives.h>
#include <iostream>

class rect {
    private:
        int x, y, w, h;
    public:
        //Konstruktor
        rect(int x=0, int y=0, int w=0, int h=0):x(x),y(y),w(w),h(h) {}
        //Setterek
        void setX(int x) { this->x = x; }
        void setY(int y) { this->y = y; }
        //Tagfüggvények
        void draw(SDL_Renderer *renderer, SDL_Window *window, double angle, int r, int g, int b);
};