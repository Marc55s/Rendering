#include "raylib.h"
#define BUF_SIZE 1000

//TODO: vectorize struct?

typedef struct Line3 {
    Vector3 start;
    Vector3 end;
} Line3;

typedef struct lorenz_system {
    double x,y,z; 
    double sig,rho,beta;
    double time;
    int points;
    Line3 *lines;
} Lorenz_sys;

Lorenz_sys *init(double sig, double rho, double beta,double time, double x, double y, double z, int points);

void progress(Lorenz_sys *system, int iterations);
void DrawLorenzSystem(Lorenz_sys *system, int points, Color color);
void initLines(Lorenz_sys *system, int points, int stepsBetweenPoints);
void updateLines(Lorenz_sys *system);
