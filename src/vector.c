#include "vector.h"
#include "math.h"

// set the vector to (x, y, z, 0.0)
void vector_set(Vector *v, double x, double y, double z){
    if(v == NULL){
        fprintf(stderr, "Invalid vector.\n");
        return;
    }

    v -> val[0] = x;
    v -> val[1] = y;
    v -> val[2] = z;
    v -> val[3] = 0.0;
}

// print out the Vector to stream fp in a pretty form
void vector_print(Vector *v, FILE *fp){
    if (v == NULL || fp == NULL) {
        return;
    }
    
    fprintf(fp, "Vector: (x: %.2f, y: %.2f, z: %.2f, h: %.2f)\n", 
            v->val[0], v->val[1], v->val[2], v->val[3]);
}

// copy the src Vector into the dest Vector
void vector_copy(Vector *dest, Vector *src){
    if(dest == NULL || src == NULL){
        fprintf(stderr, "Invalid source or destination vector.\n");
        return;
    }

    for(int i = 0; i < 4; i++){
        dest -> val[i] = src -> val[i];
    }
}

// returns the Euclidean length of the vector. The homogeneous coordinate should be 0
double vector_length(Vector *v){
    if(v == NULL){
        fprintf(stderr, "Invalid vector.\n");
        return 0;
    }

    double x = v -> val[0];
    double y = v -> val[1];
    double z = v -> val[2];

    return sqrt(x*x + y*y + z*z);
}

// normalize the vector to unit length. Do not modify the homogeneous coordinate
void vector_normalize(Vector *v){
    if(v == NULL){
        fprintf(stderr, "Invalid vector.\n");
        return;
    }

    double l = vector_length(v);

    for(int i = 0; i < 3; i++){
        v->val[i] = v->val[i] / l;
    }
}

// return the scalar product of vectors a and b
double vector_dot(Vector *a, Vector *b){
    if(a == NULL || b == NULL){
        fprintf(stderr, "Invalid vectors.\n");
        return 0;
    }

    return a->val[0] * b->val[0] + a->val[1] * b->val[1] + a->val[2] * b->val[2];
}

// calculate the cross product(vector product) of a and b and put the result in c
void vector_cross(Vector *a, Vector *b, Vector *c){
    if(a == NULL || b == NULL || c == NULL){
        fprintf(stderr, "Invalid vectors.\n");
        return;
    }

    c->val[0] = a->val[1] * b->val[2] - a->val[2] * b->val[1];
    c->val[1] = a->val[2] * b->val[0] - a->val[0] * b->val[2];
    c->val[2] = a->val[0] * b->val[1] - a->val[1] * b->val[0];
    c->val[3] = 0.0;
}

void vector_add(Vector *res, Vector *a, Vector *b){
    res->val[0] = a->val[0] + b->val[0];
    res->val[1] = a->val[1] + b->val[1];
    res->val[2] = a->val[2] + b->val[2];
    res->val[3] = 0.0;
}

void vector_subtract(Vector *res, Vector *a, Vector *b){
    res->val[0] = a->val[0] - b->val[0];
    res->val[1] = a->val[1] - b->val[1];
    res->val[2] = a->val[2] - b->val[2];
    res->val[3] = 0.0;
}

void vector_scale(Vector *v, double s){
    v->val[0] *= s;
    v->val[1] *= s;
    v->val[2] *= s;
}
