//TODO: vectorize struct?
typedef struct lorenz_system{
    double x,y,z; 
    double sig,rho,beta;
    double time;
} Lorenz_sys;

Lorenz_sys *init(double sig, double rho, double beta,double time, double x, double y, double z);

void progress(Lorenz_sys *system, int iterations);

