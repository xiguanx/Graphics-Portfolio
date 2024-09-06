#ifndef POLYLINE_H
#define POLYLINE_H
#include <stdio.h>
#include "image.h"
#include "point.h"

typedef struct{
    int zBuffer; // whether to use the z-buffer, default to true 1
    int numVertex; // number of vertices
    Point *vertex; // vertex info
}Polyline;

Polyline *polyline_create();
Polyline *polyline_createp(int numV, Point *vlist);
void polyline_free(Polyline *p);
void polyline_init(Polyline *p);
void polyline_set(Polyline *p, int numV, Point *vlist);
void polyline_clear(Polyline *p);
void polyline_zBuffer(Polyline *p, int flag);
void polyline_copy(Polyline *to, Polyline *from);
void polyline_print(Polyline *p, FILE *fp);
void polyline_normalize(Polyline *p);
void polyline_draw(Polyline *p, Image *src, Color c);

#endif