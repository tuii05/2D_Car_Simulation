#include <iostream>

#ifndef RADAR_H
#define RADAR_H

class Auto
{
private:
    int x, y, L;
    double k_r, k_d;
    int m;
    double g = 9.81 * 10;   // 10 pixel 1 méter
    double theta, delta, v; // L = tengelytáv
    int ts;                 // végsebesség
    int acc;
    int dec;

public:
    // Konstruktor
    Auto(int x = 0, int y = 0, int L = 1, double theta = 0, double delta = 0, double v = 2, double k_r = 0.0121, double k_d = 0.25, int m = 1500, int ts = 540) : x(x), y(y), L(L), theta(theta), delta(delta), v(v), k_r(k_r), k_d(k_d), m(m), ts(ts) {}
    // Getterek
    int getX() { return x; }
    int getY() { return y; }
    double getV() { return v; }
    double getTheta() { return theta; }
    double getDelta() { return delta; }
    double getL() { return L; }
    // Setterek
    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
    void setV(double v) { this->v = v; }
    void setTheta(double theta) { this->theta = theta; }
    void setDelta(double delta) { this->delta = delta; }
    void setL(double L) { this->L = L; }
    // Tagfüggvények
    void newcoord(double deltat);
};
#endif