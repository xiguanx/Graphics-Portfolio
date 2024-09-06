#ifndef MODULE_H
#define MODULE_H
#include "color.h"
#include "point.h"
#include "polygon.h"
#include "matrix.h"
#include "lighting.h"
#include "drawstate.h"
#include "bezier.h"

typedef enum {
    ObjNone,
    ObjLine,
    ObjPoint,
    ObjPolyline,
    ObjPolygon,
    ObjIdentity,
    ObjMatrix,
    ObjColor,
    ObjBodyColor,
    ObjSurfaceColor,
    ObjSurfaceCoeff,
    ObjLight,
    ObjModule
}ObjectType;

// element structure
typedef struct{
    ObjectType type; // type of object stored in the obj pointer
    void *obj;       // pointer to the object
    void *next;      // next pointer
}Element;

// Module structure
typedef struct{
    Element *head;
    Element *tail;
}Module;

// 2D module functions
Element *element_create();
Element *element_init(ObjectType type, void *obj);
void element_delete(Element *e);
Module *module_create();
void module_clear(Module *md);
void module_delete(Module *md);
void module_insert(Module *md, Element *e);
void module_module(Module *md, Module *sub);
void module_point(Module *md, Point *p);
void module_line(Module *md, Line *p);
void module_polyline(Module *md, Polyline *p);
void module_polygon(Module *md, Polygon *p);
void module_identity(Module *md);
void module_translate2D(Module *md, double tx, double ty);
void module_scale2D(Module *md, double sx, double sy);
void module_rotateZ(Module *md, double cth, double sth);
void module_shear2D(Module *md, double shx, double shy);
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src);

// 3D module functions
void module_translate(Module *md, double tx, double ty, double tz);
void module_scale(Module *md, double sx, double sy, double sz);
void module_rotateX(Module *md, double cth, double sth);
void module_rotateY(Module *md, double cth, double sth);
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w);

// shading/color module functions
void module_color(Module *md, Color *c);
void module_bodyColor(Module *md, Color *c);
void module_surfaceColor(Module *md, Color *c);
void module_surfaceCoeff(Module *md, float coeff);
void module_addLight(Module *m, Light *light);
//void module_parseLighting(Module *m, Matrix *GTM, Lighting *lighting);

// Bezier Curve and Surface Module Functions
void module_bezierCurve(Module *m, BezierCurve *b, int divisions);
void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid);

// 3D shapes
void module_cube(Module *md, int solid);
void module_cylinder( Module *mod, int sides );
void module_cone(Module *mod, int sides);
void module_sphere(Module *mod, int slices, int stacks);
void module_prism( Module *mod );

#endif
