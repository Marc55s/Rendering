#include "lorenz.h"
#include "raylib.h"

#define POINT_AMOUNT 1000

Lorenz_sys *init(double sig, double rho, double beta,double time, double x, double y, double z, int points){
    Lorenz_sys *system = (Lorenz_sys *) MemAlloc(sizeof(Lorenz_sys));
    system->x = x;
    system->y = y;
    system->z = z;
    system->sig = sig;
    system->rho = rho;
    system->beta = beta;
    system->time = time;
    system->points = points;
    system->lines = (Line3*) MemAlloc(sizeof(Line3) * points);

    initLines(system, points, 1);

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

void initLines(Lorenz_sys *system, int points, int stepsBetweenPoints){
    TraceLog(LOG_INFO, "POINTS ignored");
    for (int i = 0; i < points; i++) {
        Vector3 s = (Vector3){system->x,system->y,system->z};
        progress(system, 1);
        system->lines[i] = (Line3){s,(Vector3){system->x,system->y,system->z}};
    }
}

void updateLines(Lorenz_sys *system){
    for (int i = 0; i < (system->points)-1; i++) {
        system->lines[i] = system->lines[i+1];
    }
    Vector3 s = (Vector3){system->x,system->y,system->z};
    progress(system, 1);
    system->lines[system->points-1] =(Line3){s,(Vector3){system->x,system->y,system->z}}; 
}

void DrawLorenzSystem(Lorenz_sys *system, int points, Color color){
    Line3 *lines = system->lines;
    for (int i = 0; i < system->points; i++) {
        Line3 line = lines[i];
        DrawLine3D(line.start, line.end, color);
    }
}
