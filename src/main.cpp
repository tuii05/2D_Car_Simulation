// mingw32-make -f Makefile; start Main.exe

#include <SDL.h>
#include <iostream>
#include "screen.h"

int main(int argc, char *argv[])
{
    std::cout << "SDL megy. \n";
    Auto auto1(0, 0, 1, 0, 0, 200, 0.012, 0.25, 1500, 540); // Autó kezdeti paramétereinek megadása.
    Screen kep1(1920, 1080, 20.0); // A képernyő létrehozása, első két paraméter: szélesség x magasság.
    kep1.abrazol(auto1, 200, 80, 60.0); // Ábrázolás.
    return 0;
}
