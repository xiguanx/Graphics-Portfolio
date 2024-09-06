#ifndef LIGHTING_H
#define LIGHTING_H
#include "color.h"
#include "point.h"
#include "vector.h"

typedef enum{
  LightNone,
  LightAmbient,
  LightDirect,
  LightPoint,
  LightSpot,
}LightType;

typedef struct{
  LightType type;
  Color color;
  Vector direction;
  Point position;
  float cutoff; // store the cosine of the cutoff angle of a spotlight
  float sharpness; // coefficient of the falloff function(power for cosine)
}Light;

typedef struct{
  int nLights;
  Light light[64];
} Lighting;

//light functions
void light_init(Light *light);
void light_copy(Light *to, Light *from);
void light_set(Light *light, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness);

//lighting functions
Lighting *lighting_create(void);
void lighting_delete(Lighting *lights);
void lighting_init(Lighting *l);
void lighting_clear(Lighting *l);
void lighting_add(Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness);
void lighting_shading(Lighting *l, Vector *N, Vector *V, Point *p, Color *Cb, Color *Cs, float s, int oneSided, Color *c);
void lighting_copy(Lighting *to, Lighting *from);

#endif