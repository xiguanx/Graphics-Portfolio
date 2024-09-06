#ifndef DRAWSTATE_H
#define DRAWSTATE_H
#include "color.h"
#include "point.h"

typedef enum{
    ShadeFrame, // draw only the borders of objects, including polygons
    ShadeConstant, // draw objects using the current foreground color, fill polygons
    ShadeDepth, // draw objects using their depth value
    ShadeFlat, // draw objects using shading calculations, but each polygon is a constant value
    ShadeGouraud, // draw objects using gouraud shading
    ShadePhong // draw objects using Phong shading
}ShadeMethod;

typedef struct{
    Image *i;
    float s, t;
}Texture;

typedef struct{
    Color color; // the foreground color, used in the default drawing mode
    Color flatColor; // the color to flat-fill a polygon based on a shading calculation
    Color bodyColor; // the body reflection color, used for shading calculations
    Color surfaceColor; // the surface reflection color, used for shading calculations
    float surfaceCoeff; // a float that represents the shininess of the surface
    ShadeMethod shade; // an enumerated type ShadeMethod
    int zBufferFlag; // whether to use z-buffer hidden surface removal
    Point viewer; // a point representing the view location in 3D(identical to the VRP in View3D)
    Texture texture;
}DrawState;

DrawState *drawstate_create(void);
void drawstate_setColor(DrawState *s, Color c);
void drawstate_setBody(DrawState *s, Color c);
void drawstate_setSurface(DrawState *s, Color c);
void drawstate_setSurfaceCoeff(DrawState *s, float f);
void drawstate_copy(DrawState *to, DrawState *from);
void drawstate_print(DrawState *s);

#endif