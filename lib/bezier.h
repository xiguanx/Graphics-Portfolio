#ifndef BEZIER_H
#define BEZIER_H
#include "color.h"
#include "point.h"
#include "image.h"

typedef struct{
    Point p[4];
    int zBuffer;
}BezierCurve;

typedef struct{
    Point p[16];
    int zBuffer;
}BezierSurface;

void bezierCurve_init(BezierCurve *b);
void bezierSurface_init(BezierSurface *b);
void bezierCurve_set(BezierCurve *b, Point *vlist);
void bezierSurface_set(BezierSurface *b, Point *vlist);
void bezierCurve_zBuffer(BezierCurve *p, int flag);
void bezierSurface_zBuffer(BezierCurve *p, int flag);
void bezierCurve_draw(BezierCurve *b, Image *src, Color c);
float getLargestDimension(BezierCurve *b);
void de_casteljau(BezierCurve *b, BezierCurve *left, BezierCurve *right);

#endif