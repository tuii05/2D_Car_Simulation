#include "Radar.h"
#include <iostream>
#include <SDL2_gfxPrimitives.h>

// Tagfüggvény, amely megvizsgálja, hogy egy adott pixel fehér-e.
bool radar::isWhite(int x, int y)
{
    Uint32 *pixels = (Uint32 *)surface->pixels;
    Uint32 pixel = pixels[(y * surface->w) + x];
    Uint8 r, g, b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
    return (r == 255 && g == 255 && b == 255);
}

// Az akadályok keresését végző tagfüggvény, meghatározza, hogy a szimulált radar hatótávolságjában van-e akadály.
bool radar::detectObs(int x0, int y0, double angle, int hatotav)
{
    double rad = angle * M_PI / 180;
    for (int i = 0; i < hatotav; ++i)
    {
        int x = round(x0 + i * cos(rad));
        int y = round(y0 + i * sin(rad));
        if (x < 0 || x >= surface->w || y < 0 || y >= surface->h)
        {
            return false;
        }
        if (isWhite(x, y) == true)
        {
            return true;
        }
    }
    return false;
}

// Megkeresi az akadály pontjait egy adott helyen, majd visszaadja, hogy talált-e vagy sem.
bool radar::detectPoints(int x0, int y0, double rad, Coords &adat, double max)
{
    int x1;
    int y1;
    for (int i = 1; i < max; ++i)
    {
        x1 = round(x0 + i * cos(rad));
        y1 = round(y0 + i * sin(rad));
        if (x1 < surface->w && x1 > 0 && y1 < surface->h && y1 > 0)
        {
            if (isWhite(x1, y1))
            {
                adat.x = x1;
                adat.y = y1;
                i = max;
                return true;
            }
        }
        else
        {
            // std::cout << "\nHiba! : Nem található pont. \n";
            return false;
        }
    }
    return false;
}

// Többféle hajlásszögű radar szempontjából keresi az akadály pontjait, és megvizsgálja azokat, majd a pontok halmazát adja vissza.
savadat radar::savDetect(int x0, int y0, int w, int h, int w_r, int h_r, int angle)
{
    savadat savadatok;
    int max = sqrt(w * w + h * h);
    bool helyes;
    for (int i = -90; i <= 90; i += 30)
    {
        double rad = (angle + i) * M_PI / 180;
        Coords adat;
        helyes = detectPoints(x0, y0, rad, adat, max);
        if (helyes == true)
        {
            savadatok.push_back(adat);
        }
    }
    return savadatok;
}

// Kiszámítja a szükséges kormányzási szöget egyszerű PID szabályzó és geometria használatával.
double radar::deltaszam(int x0, int y0, int theta0, savadat savadatok, double &angle_err, double &dist_err, Auto &auto1)
{ // r21 -> savadatok[3], r11 -> savadatok[1]
    // I tényező nagysága
    static double integral_err = 0.0;
    // P, D, I tényező
    double k_p = 0.005;  // 0.005
    double k_d = 0.1375; // 0.1375
    double k_i = 0.003;  // 0.004545
    //std::cout << "Meret = " << savadatok.size() << "\n";
    if (savadatok.size() <= 4)
    {
        //std::cout << "Hiba! \n";
        return 0;
    }
    double xk1 = (savadatok[0].x + savadatok[savadatok.size() - 1].x) / 2.0;
    double yk1 = (savadatok[0].y + savadatok[savadatok.size() - 1].y) / 2.0;

    double xk2 = (savadatok[1].x + savadatok[savadatok.size() - 2].x) / 2.0;
    double yk2 = (savadatok[1].y + savadatok[savadatok.size() - 2].y) / 2.0;
    // A távolságból származó hiba
    double diff = (xk2 - xk1) * (yk1 - y0) - (yk2 - yk1) * (xk1 - x0);
    double hossz = sqrt((xk2 - xk1) * (xk2 - xk1) + (yk2 - yk1) * (yk2 - yk1));
    dist_err = diff / hossz;
    double valos_tav = sqrt(((xk1 + xk2) / 2 - x0) * ((xk1 + xk2) / 2 - x0) + ((yk1 + yk2) / 2 - y0) * ((yk1 + yk2) / 2 - y0));
    // Integrál tényező
    if (std::abs(integral_err) < 15)
    {
        integral_err += dist_err * 20.0 / 1000.0;
    }
    // A szögből származó hiba
    double lane_angle = atan2(yk2 - yk1, xk2 - xk1);
    double car_angle = theta0 * M_PI / 180.0;
    angle_err = -(car_angle - lane_angle);
    double ptm = 1.0 / 10.0;
    // Kormányzási szög kiszámítása
    double delta_jo = k_p * dist_err * std::abs(1 / dist_err) * valos_tav * ptm + k_d * angle_err + k_i * integral_err;
    double delta = delta_jo;
    return delta * 180.0 / M_PI;
}