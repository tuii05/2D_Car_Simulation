#include "auto.h"
#include "Radar.h"
#include <math.h>
#include <iostream>

#undef __STRICT_ANSI__
#include <cmath>


//Kiszámolja az autó változóinak az új értékeit a bicikli modellből.
void Auto::newcoord(double deltat)
{
    double curr_theta;
    curr_theta = theta;
    // Lenormáljuk a thetát, ha kimegy a [-360, 360] intervallumból, és kiszámoljuk az újat.
    if (theta <= 360 && theta >= 0) {
        theta = theta + v / L * tan(delta * M_PI / 180) * deltat;
    }
    else if (theta >= 360 && theta >= 0){
        theta = theta + v / L * tan(delta * M_PI / 180) * deltat;
        curr_theta = theta;
        theta = curr_theta - ((int(curr_theta / 360.0)) * 360);
    }
    else if(theta < 0 && theta >= -360) {
        theta = theta + v / L * tan(delta * M_PI / 180) * deltat;
    }
    else {
        theta = theta + v / L * tan(delta * M_PI / 180) * deltat;
        curr_theta = theta;
        theta = std::abs(curr_theta) - ((int(curr_theta / 360.0)) * 360);
        theta = -theta;
    }
    // új (x,y) koordináták kiszámolása.
    x = x + v * cos(curr_theta * M_PI / 180) * deltat;
    y = y + v * sin(curr_theta * M_PI / 180) * deltat;
    //v = v + a * deltat;
    /*v = v - (k_r * g + k_d * v * v/m) * deltat;
    if (v < 0) {
        v = 0;
    }*/ //súrlódás + gördülési ellenállás
    /*std::cout << "\n deltat = " << deltat << "\n";
    std::cout << v << " & " << v * cos(curr_theta * 3.14 / 180) * deltat << "\n";
    std::cout << x << ", " << y << "\n";
    std::cout << "\n L = " << L;*/
}