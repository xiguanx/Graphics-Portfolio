#include "point.h"

// set the first two values of the vector to x and y
// set the third value to 0.0 and the fourth value to 1.0
void point_set2D(Point *p, double x, double y){
    if(p == NULL){
        fprintf(stderr, "Invalid point.");
    }

    p -> val[0] = x;
    p -> val[1] = y;
    p -> val[2] = 1.0;
    p -> val[3] = 1.0;
}

// set the point's value to x, y and z.
// set the homogeneous coordinate to 1.0
void point_set3D(Point *p, double x, double y, double z){
    if(p == NULL){
        fprintf(stderr, "Invalid point.");
    }

    p -> val[0] = x;
    p -> val[1] = y;
    p -> val[2] = z;
    p -> val[3] = 1.0;
}

//set the four values to x, y, z and h
void point_set(Point *p, double x, double y, double z, double h){
    if(p == NULL){
        fprintf(stderr, "Invalid point.");
    }

    p -> val[0] = x;
    p -> val[1] = y;
    p -> val[2] = z;
    p -> val[3] = h;
}

// normalize the x and y values of a point by its homogeneous coordinate x = x/h, y = y/h
void point_normalize(Point *p){
    if(p == NULL){
        fprintf(stderr, "Invalid point.");
    }

    if(p -> val[3] == 0){
        fprintf(stderr, "Invalid weight. Unable to normalize point.\n");
        return;
    }

    p -> val[0] = p -> val[0] / p -> val[3];
    p -> val[1] = p -> val[1] / p -> val[3];
    p -> val[2] = p -> val[2] / p -> val[3];
    p -> val[3] = 1;
}

// copy the data structure
void point_copy(Point *to, Point *from){
    if(to == NULL || from == NULL){
        fprintf(stderr, "Unable to copy.");
    }

    to -> val[0] = from -> val[0];
    to -> val[1] = from -> val[1];
    to -> val[2] = from -> val[2];
    to -> val[3] = from -> val[3];
}

// draw the point into src using color c
void point_draw(Point *p, Image *src, Color c){
    if (p == NULL || src == NULL) {
        fprintf(stderr, "Unable to draw point. Invalid point or image.");
        return;
    }

    int x = (int)(p->val[0]);
    int y = (int)(p->val[1]);

    if (x >= 0 && x < src->cols && y >= 0 && y < src->rows) {
        src->data[y][x].c = c;
        src->data[y][x].a = 1.0;  // Assuming full opacity for the point.
    } else {
        fprintf(stderr, "Point coordinates out of image bounds.");
    }
}

// draw the point into src using FPixel c
void point_drawf(Point *p, Image *src, FPixel c){
    if (p == NULL || src == NULL) {
        fprintf(stderr, "Unable to draw point. Invalid point or image.");
        return;
    }

    int x = (int)(p->val[0]);
    int y = (int)(p->val[1]);

    if (x >= 0 && x < src->cols && y >= 0 && y < src->rows) {
        src->data[y][x] = c;
    } else {
        fprintf(stderr, "Point coordinates out of image bounds.");
    }
}

// use fprintf to print the contents of the point to the stream fp.
void point_print(Point *p, FILE *fp){
    if (p == NULL || fp == NULL) {
        fprintf(stderr, "Unable to print point. Invalid point or file stream.");
        return;
    }

    fprintf(fp, "Point: ( %lf, %lf, %lf, %lf )\n", p->val[0], p->val[1], p->val[2], p->val[3]);
}