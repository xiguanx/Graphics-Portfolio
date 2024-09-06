#ifndef VIEW2D_H
#define VIEW2D_H
#include "point.h"
#include "vector.h"

typedef struct{
    Point vrp; // center of the view rectangle V0 in world coordinates
    double du; // width of the view rectangle du in world coordinates
    Vector x; // the orientation angle θv or the x-axis of the view window expresssed as a normalized vector(nx, ny) = (cos(θv), sin(θv)). 
    int screenx; // the number of columns C in the output image;
    int screeny; // the number of rows R in the output image;
}View2D;

void view2D_set(View2D *view, Point *vrp, double du, Vector *x, int screenx, int screeny);

#endif