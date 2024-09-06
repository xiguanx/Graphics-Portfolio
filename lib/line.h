#ifndef LINE_H
#define LINE_H
#include "color.h"
#include "image.h"
#include "point.h"

typedef struct{
    int zBuffer; // depth, default to true(1)
    Point a; // starting point
    Point b; // ending point
}Line;

void line_set2D(Line *l, double x0, double y0, double x1, double y1);
void line_set(Line *l, Point ta, Point tb);
void line_zBuffer(Line *l, int flag);
void line_normalize(Line *l);
void line_copy(Line *to, Line *from);
void line_draw(Line *l, Image *src, Color c);
void line_drawDash(Line *l, Image *src, Color c, int length);

#endif