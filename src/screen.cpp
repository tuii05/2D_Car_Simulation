#include "screen.h"
#include "Assist.h"
#include <iostream>
#include <SDL_keyboard.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_keycode.h>
#include <vector>
#include <math.h>

// Szükséges renderer és window pointerek elkészítése az ablakhoz és az ábrázoláshoz.
void Screen::sdl_init(SDL_Window **pwindow, SDL_Renderer **prenderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "Nem indithato az SDL: " << SDL_GetError();
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("Auto szimulacio", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
    if (window == NULL)
    {
        std::cout << "Nem hozhato letre az ablak: " << SDL_GetError();
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
    {
        std::cout << "Nem hozhato letre a megjelenito: " << SDL_GetError();
        exit(1);
    }
    SDL_RenderClear(renderer);
    *pwindow = window;
    *prenderer = renderer;
}

// Automatikusan eventet küld, szükséges az autó mozgatásához.
Uint32 idozit(Uint32 ms, void *param)
{
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

// Pozíció figyelése --> ha kimegy a képről, akkor ne tünjön el, hanem kerüljön vissza
void checkpos(int x, int y, int w, int h, int w_r, int h_r, rect &Rect, Auto &auto1, double deltat_ms)
{
    if (x < w && y < h && y > 0 && x > 0)
    {
        Rect.setX(x);
        Rect.setY(y);
    }
    else if (x > w && (y < h && y > 0))
    {
        auto1.setX(w_r / 2);
        Rect.setX(w_r / 2);
        Rect.setY(auto1.getY());
    }
    else if (x < 0 && (y < h && y > 0))
    {
        auto1.setX(w - w_r / 2);
        Rect.setX(w - w_r / 2);
        Rect.setY(auto1.getY());
    }
    else if ((x < w && x > 0) && y > h)
    {
        auto1.setY(h_r / 2);
        Rect.setX(auto1.getX());
        Rect.setY(h_r / 2);
    }
    else if ((x < w && x > 0) && y < 0)
    {
        auto1.setY(h - h_r / 2);
        Rect.setX(auto1.getX());
        Rect.setY(h - h_r / 2);
    }
    else if (x > w && y > h)
    {
        auto1.setX(w_r / 2);
        auto1.setY(h_r / 2);
        Rect.setX(w_r / 2);
        Rect.setY(h_r / 2);
    }
    else if (x < 0 && y < 0)
    {
        auto1.setX(w - w_r / 2);
        auto1.setY(h - h_r / 2);
        Rect.setX(w - w_r / 2);
        Rect.setY(h - h_r / 2);
    }
}

// Kanyargós út kirajzolásához szükséges
void Screen::DrawSine(SDL_Renderer *renderer, double amp, double freq, int y0, int r, int g, int b)
{
    double theta;
    int y;
    for (int x = 0; x < w; ++x)
    {
        theta = 2 * M_PI * freq * x;
        y = y0 + int(amp * sin(theta));
        if (y > 0 && y < h)
        {
            pixelRGBA(renderer, x, y, r, g, b, 0xFF);
        }
    }
}

// Az autó (kék téglalap) újrarajzolását végzi, szükséges az autó mozgásának ábrázolásához.
void Screen::Redraw(SDL_Renderer *renderer, SDL_Window *window, Auto &auto1, rect rect, int w_r, int h_r, double angle, double delta, int *rasegites, const double steerang)
{
    double deltat_ms = dt_ms;
    auto1.setDelta(delta);
    int x0, y0, x, y;
    x0 = auto1.getX();
    y0 = auto1.getY();
    Assists assist(x0, y0, w, h, w_r, h_r, steerang);
    auto1.newcoord(deltat_ms / 1000);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    rect.draw(renderer, window, angle, 0, 0, 0);
    x = auto1.getX() - w_r / 4; // w_r/2
    y = auto1.getY() - h_r / 4; // h_r/2
    // Pozíció ellenőrzése
    checkpos(x, y, w, h, w_r, h_r, rect, auto1, deltat_ms);
    //
    angle = auto1.getTheta();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    rect.draw(renderer, window, angle, 0, 255, 255);
    SDL_RenderPresent(renderer);
    // Út kirajzolása
    thickLineRGBA(renderer, 0, h / 4, w, h / 4, 1, 255, 255, 255, 0xFF);
    thickLineRGBA(renderer, 0, 3 * h / 4, w, 3 * h / 4, 1, 255, 255, 255, 0xFF);
    thickLineRGBA(renderer, 0, h / 2, w, h / 2, 1, 255, 255, 0, 0xFF);
    // DrawSine(renderer, 150, 0.2/360, h/4, 255, 255, 255);
    // DrawSine(renderer, 150, 0.2/ 360, 3*h/4, 255, 255, 255);
    // DrawSine(renderer, 150, 0.2 / 360, h/2, 0, 255, 255);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
    //assist.tavolsagtartas(auto1, renderer, window, rasegites);
    assist.savtartas(auto1, renderer, window, rasegites); // Ha távolságtartást akarjuk kipróbálni, akkor ezt rakjuk commentbe.
}

// Ellenőrzi, hogy rásegítés van-e érvényben, és annak függvényében állítja be a kormányzási szöget (steering angle).
void checkrasegit(const int rasegites, double *delta, const double steerang, int esemeny, Auto &auto1)
{
    if (rasegites == 0)
    {
        if (esemeny == 1)
        {
            *delta = 0;
        }
        else if (esemeny == 2)
        {
            *delta = -steerang;
        }
        else if (esemeny == 3)
        {
            *delta = steerang;
        }
    }
    else if (rasegites == 1)
    {
        *delta = -steerang;
    }
    else if (rasegites == 2)
    {
        *delta = +steerang;
    }
    else if (rasegites == 3)
    {
        *delta = auto1.getDelta();
    }
}

//Az osztály fő tagfüggvénye, ez kezeli az eventeket és az eventekre való válaszokat.
void Screen::abrazol(Auto auto1, int w_r, int h_r, const double steerang)
{ // 10 pixel = 1 meter
    double delta = steerang;
    // Ablak és renderer elkészítése.
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init(&window, &renderer);
    double deltat_ms = dt_ms;
    SDL_TimerID id = SDL_AddTimer(deltat_ms, idozit, NULL);
    // Kezdeti pozíció beállítása.
    auto1.setX(0 + 10);     // w/2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    auto1.setY(h / 2 + 10); // h/2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Alap adatok megadása.
    double acc = 2.5;
    rect rect(0 + 10, h / 2 + 10, w_r, h_r); // w/2 h/2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    double angle = auto1.getTheta();
    // Kirajzoljuk a kezdeti állapotot.
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    rect.draw(renderer, window, angle, 0, 255, 255);
    // Akadályok kirajzolása.
    thickLineRGBA(renderer, 0, h / 4, w, h / 4, 1, 255, 255, 255, 0xFF);
    thickLineRGBA(renderer, 0, 3 * h / 4, w, 3 * h / 4, 1, 255, 255, 255, 0xFF);
    // DrawSine(renderer, 150, 0.2 / 360, w, h, h / 4, 255, 255, 255);
    // DrawSine(renderer, 150, 0.2 / 360, w, h, 3 * h / 4, 255, 255, 255);
    // DrawSine(renderer, 150, 0.2 / 360, w, h, h / 2, 0, 255, 255);
    SDL_RenderPresent(renderer);
    // A szimuláció lebonyolítása.
    bool isRunning = true;
    int rasegites = 0;
    int esemeny;
    while (isRunning)
    {
        int x0, y0;
        x0 = auto1.getX();
        y0 = auto1.getY();
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        // A kapott event kiértékelése.
        switch (ev.type)
        {
        case SDL_USEREVENT: // Alap event, ha nem történik semmi, szükséges, hogy az autó mindenképpen mozogjon.
            esemeny = 1;
            checkrasegit(rasegites, &delta, steerang, esemeny, auto1);
            Redraw(renderer, window, auto1, rect, w_r, h_r, angle, delta, &rasegites, steerang);
            SDL_RenderPresent(renderer);
            break;
        case SDL_KEYDOWN:
            switch (ev.key.keysym.sym)
            {
            case SDLK_a: //Balra kormányzás.
                esemeny = 2;
                checkrasegit(rasegites, &delta, steerang, esemeny, auto1);
                Redraw(renderer, window, auto1, rect, w_r, h_r, angle, delta, &rasegites, delta);
                SDL_RenderPresent(renderer);
                break;
            case SDLK_d: //Jobbra kormányzás.
                esemeny = 3;
                checkrasegit(rasegites, &delta, steerang, esemeny, auto1);
                Redraw(renderer, window, auto1, rect, w_r, h_r, angle, delta, &rasegites, delta);
                SDL_RenderPresent(renderer);
                break;
            }
        case SDL_KEYUP:
            switch (ev.key.keysym.sym)
            {
            case SDLK_a:
                esemeny = 1;
                checkrasegit(rasegites, &delta, steerang, esemeny, auto1);
                auto1.setDelta(delta);
                // tavolsagtartas(x0, y0, w, h, rect, auto1, deltat_ms, akadaly, renderer, window, w_r, h_r, delta, &rasegites, steerang);
                // savtartas(x0, y0, w, h, rect, auto1, deltat_ms, akadaly, renderer, window, w_r, h_r, delta, &rasegites, steerang);
                break;
            case SDLK_d:
                esemeny = 1;
                checkrasegit(rasegites, &delta, steerang, esemeny, auto1);
                auto1.setDelta(delta);
                // tavolsagtartas(x0, y0, w, h, rect, auto1, deltat_ms, akadaly, renderer, window, w_r, h_r, delta, &rasegites, steerang);
                // savtartas(x0, y0, w, h, rect, auto1, deltat_ms, akadaly, renderer, window, w_r, h_r, delta, &rasegites, steerang);
                break;
            }
            break;
        case SDL_QUIT: // Ha bezárjuk az ablakot, akkor leáll.
            std::cout << "Bezart!!";
            isRunning = false;
            break;
        }
    }
    SDL_RemoveTimer(id);
    SDL_DestroyWindow(window);
    SDL_Quit();
}