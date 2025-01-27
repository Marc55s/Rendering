#include "lorenz.h"
#include "raylib.h"

Lorenz_sys *init(double sig, double rho, double beta,double time, double x, double y, double z){
    Lorenz_sys *system = (Lorenz_sys *) MemAlloc(sizeof(Lorenz_sys));
    system->x = x;
    system->y = y;
    system->z = z;
    system->sig = sig;
    system->rho = rho;
    system->beta = beta;
    system->time = time;
    return system;
}

void progress(Lorenz_sys *system, int iterations) {
    // position
    double x = system->x;
    double y = system->y;
    double z = system->z;

    // paramters
    double a = system->sig;
    double b = system->rho;
    double c = system->beta;

    double t = system->time;

    double xt = 0;
    double yt = 0;
    double zt = 0;
    for (int i = 0; i < iterations; i++) {
        xt = x + t * a * (y - x);
        yt = y + t * (x * (b - z) - y);
        zt = z + t * (x * y - c * z);
    }

    system->x = xt;
    system->y = yt;
    system->z = zt;
}
