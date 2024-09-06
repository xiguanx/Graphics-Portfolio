#ifndef VIEW3D_H
#define VIEW3D_H
#include "point.h"
#include "vector.h"

typedef struct{
    Point vrp; // view reference point: 3D vector indicating the origin of the view reference coordinates
    Vector vpn; // view plane normal: 3D vector indicating the direction in which the viewer is looking
    Vector vup; // view up vector: 3D vector indicating the up direction on the view plane. the only restriction is that it cannot be parallel to the vpn
    double d; // projection distance: distance in the negative vpn direction at which the center of projection is located
    double du; // view window extent[du, dv]: extent of view plane around the vrp, expressed in world coordinate distances
    double dv; 
    double f; // front and back clip planes[f, b]: front and back clip planes expressed as distances along the positive vpn. f>0 and f<b
    double b;
    int screenx; // screen size[x, y]: size of desired image in pixels
    int screeny;
}View3D;

#endif