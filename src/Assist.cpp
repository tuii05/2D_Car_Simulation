#include "Assist.h"

// Rásegítés, amely megakadályozza, hogy a vezető a sávból elmenjen, szituációfüggő a müködése.
void Assists::tavolsagtartas(Auto &auto1, SDL_Renderer *renderer, SDL_Window *window, int *rasegites)
{
    // A radar szimulálásához szükséges felület létrehozása.
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA8888);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, surface->pixels, surface->pitch);
    radar radar1(w, h, surface);
    // A radarokból akadály detektálása.
    double hatotav = 50 + h_r;
    bool elol;
    bool bal;
    bool jobb;
    elol = radar1.detectObs(auto1.getX(), auto1.getY(), auto1.getTheta(), hatotav);
    bal = radar1.detectObs(auto1.getX(), auto1.getY(), auto1.getTheta() - 45, hatotav);
    jobb = radar1.detectObs(auto1.getX(), auto1.getY(), auto1.getTheta() + 45, hatotav);
    // A radarokból kapott jelek vizsgálata.
    if (bal == true && jobb == false && (elol == true || elol == false))
    { // bal -, jobb +
        // std::cout << "Talalt! \n";
        auto1.setDelta(steerang);
        *rasegites = 2;
    }
    else if (bal == false && jobb == true && (elol == true || elol == false))
    {
        // std::cout << "Talalt! Jobbra: sin(theta) =" << sin(auto1.getTheta() * M_PI / 180) << "\n";

        auto1.setDelta(-steerang);
        *rasegites = 1;
    }
    else if (bal == false && jobb == false && elol == true)
    {
        if (sin(auto1.getTheta() * M_PI / 180) > 0)
        {
            auto1.setDelta(steerang);
            // std::cout << "Talalt! Jobbra: sin(theta) =" << sin(auto1.getTheta() * M_PI / 180) << "\n";
            *rasegites = 2;
        }
        else
        {
            auto1.setDelta(-steerang);
            // std::cout << "Talalt! Balra: sin(theta) =" << cos(auto1.getTheta() * M_PI / 180) << "\n";
            *rasegites = 1;
        }
    }
    else if (bal == true && jobb == true && elol == false)
    {
        if (sin(auto1.getTheta() * M_PI / 180) > 0)
        {
            auto1.setDelta(steerang);
            // std::cout << "Talalt! Jobbra: sin(theta) =" << sin(auto1.getTheta() * M_PI / 180) << "\n";
            *rasegites = 2;
        }
        else
        {
            auto1.setDelta(-steerang);
            // std::cout << "Talalt! Balra: sin(theta) =" << sin(auto1.getTheta() * M_PI / 180) << "\n";
            *rasegites = 1;
        }
    }
    else if (bal == true && jobb == true && elol == true)
    {
        if (sin(auto1.getTheta() * M_PI / 180) > 0)
        {
            auto1.setDelta(steerang);
            std::cout << "Talalt! Jobbra: sin(theta) =" << cos(auto1.getTheta() * M_PI / 180) << "\n";
            *rasegites = 2;
        }
        else
        {
            auto1.setDelta(-steerang);
            std::cout << "Talalt! Balra: sin(theta) =" << cos(auto1.getTheta() * M_PI / 180) << "\n";
            *rasegites = 1;
        }
    }
    else if (bal == false && jobb == false && elol == false)
    {
        std::cout << "Vege! \n";
        *rasegites = 0;
    }
}

// Rásegítés, amely a sáv közepén próbálja az autót tartani, elveszi az irányítást a vezetőtől, folyamatos a működése.
void Assists::savtartas(Auto &auto1, SDL_Renderer *renderer, SDL_Window *window, int *rasegites)
{
    // Beállítjuk mekkora távolság és szög hibánál állhat le a függvény.
    double hiba_tav = 1;
    double hiba_szog = 1;
    // Autó kezdeti pozícióját eltároljuk két változóban.
    int x0 = auto1.getX();
    int y0 = auto1.getY();
    // Létrehozzuk a radarok működéséhez szükséges felületeket.
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA8888);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, surface->pixels, surface->pitch);
    // Elkészítjük a radar(okat) és adatokat nyerünk belőlük.
    radar radar1(w, h, surface);
    savadat savadatok = radar1.savDetect(x0, y0, w, h, w_r, h_r, auto1.getTheta());
    double dist_err;
    double angle_err;
    double steer;
    steer = radar1.deltaszam(x0, y0, auto1.getTheta(), savadatok, angle_err, dist_err, auto1);
    // A kapott adatokat kiértékelve egy válaszreakciót adunk vissza.
    if (std::abs(dist_err) < hiba_tav) // std::abs(angle_err) < hiba_szog &&
    {
        // std::cout << "Nem kell. \n";
        auto1.setDelta(0);
        *rasegites = 0;
    }
    else
    {
        if (std::abs(steer) <= 40)
        {
            auto1.setDelta(steer);
            // std::cout << "Eletbe lepett! 1 : " << steer << "\n";
            // std::cout << "dist = " << dist_err << "\n";
            // std::cout << "angle = " << angle_err << "\n";
        }
        else
        {
            auto1.setDelta((steer / std::abs(steer)) * 40);
            // std::cout << "Eletbe lepett! 2 : " << (steer / std::abs(steer)) * 40 << "\n";
            // std::cout << "dist = " << dist_err << "\n";
            // std::cout << "angle = " << angle_err << "\n";
        }
        *rasegites = 3;
    }
}