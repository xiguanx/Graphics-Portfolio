#ifndef VECTOR_H
#define VECTOR_H
#include "color.h"
#include "point.h"

typedef Point Vector;

void vector_set(Vector *v, double x, double y, double z);
void vector_print(Vector *v, FILE *fp);
void vector_copy(Vector *dest, Vector *src);
double vector_length(Vector *v);
void vector_normalize(Vector *v);
double vector_dot(Vector *a, Vector *b);
void vector_cross(Vector *a, Vector *b, Vector *c);
void vector_add(Vector *res, Vector *a, Vector *b);
void vector_subtract(Vector *res, Vector *a, Vector *b);
void vector_scale(Vector *v, double s);

#endif