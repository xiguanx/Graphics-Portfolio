#ifndef POLYGON_H
#define POLYGON_H
#include "color.h"
#include "point.h"
#include "vector.h"
#include "drawstate.h"
#include "lighting.h"

typedef struct{
    int oneSided; // whether to consider polygon one-sided(1) or two-sided(0) for shading
    int nVertex; // number of vertices
    Point *vertex; // vertex info
    Color *color; // color info for each vertex
    Vector *normal; // surface normal information for each vertex
    int zBuffer; // whether to use the z-buffer, default to true(1)
    Texture *texture;
    //Point *worldPos;
}Polygon;

Polygon *polygon_create();
Polygon *polygon_createp(int numV, Point *vlist);
void polygon_free(Polygon *p);
void polygon_init(Polygon *p);
void polygon_set(Polygon *p, int numV, Point *vlist);
void polygon_clear(Polygon *p);
void polygon_setSided(Polygon *p, int oneSided);
void polygon_setColors(Polygon *p, int numV, Color *clist);
void polygon_setNormals(Polygon *p, int numV, Vector *nlist);
void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist, Vector *nlist, int zBuffer, int oneSided);
void polygon_zBuffer(Polygon *p, int flag);
void polygon_copy(Polygon *to, Polygon *from);
void polygon_print(Polygon *p, FILE *fp);
void polygon_normalize(Polygon *p);
void polygon_draw(Polygon *p, Image *src, Color c);
void polygon_drawFill(Polygon *p, Image *src, Color c);
void polygon_drawFillB(Polygon *p, Image *src, Color c);
void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *light);
void polygon_shade(Polygon *p, DrawState *ds, Lighting *light);
//void polygon_drawFillGradient(Polygon *p, Image *src, Color startColor, Color endColor);

#endif