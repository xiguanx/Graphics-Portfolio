#include <math.h>
#include "bezier.h"
#include "line.h"

// sets the zbuffer flag to 1 and the curve points to the X-axis between 0 and 1
void bezierCurve_init(BezierCurve *b){
    if(b == NULL){
        fprintf(stderr, "Invalid bezier curve.\n");
        return;
    }

    b->zBuffer = 1;
    for(int i = 0; i < 4; i++){
        b->p[i].val[0] = i / 3;
        b->p[i].val[1] = 0;
        b->p[i].val[2] = 0;
        b->p[i].val[3] = 1;
    }
}

// sets the zbuffer flag to 1 and the surface to the X-Z plane between (0, 0) and (1, 1)
void bezierSurface_init(BezierSurface *b){
    if(b == NULL){
        fprintf(stderr, "Invalid bezier surface.\n");
        return;
    }
    b->zBuffer = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int index = i * 4 + j;
            b->p[index].val[0] = i / 3.0; 
            b->p[index].val[1] = 0;
            b->p[index].val[2] = j / 3.0;
            b->p[index].val[3] = 1.0;
        }
    }
}

// sets the control points of the BezierCurve to the four points in the vlist array
void bezierCurve_set(BezierCurve *b, Point *vlist){
    if(b == NULL || vlist == NULL){
        fprintf(stderr, "Invalid bezier curve or vlist.\n");
        return;
    }

    for(int i = 0; i < 4; i++){
        point_copy(&(b->p[i]), &(vlist[i]));
    }
}

// sets the control points of the BezierSurface to the 16 points in the vlist array
void bezierSurface_set(BezierSurface *b, Point *vlist){
    if(b == NULL || vlist == NULL){
        fprintf(stderr, "Invalid bezier curve or vlist.\n");
        return;
    }

    for(int i = 0; i < 16; i++){
        point_copy(&(b->p[i]), &(vlist[i]));
    }
}

// sets the z-buffer flag to the given value
void bezierCurve_zBuffer(BezierCurve *p, int flag){
    if(p == NULL){
        fprintf(stderr, "Invalid bezier curve.\n");
        return;
    }

    p->zBuffer = flag;
}

// sets the z-buffer flag to the given value
void bezierSurface_zBuffer(BezierCurve *p, int flag){
    if(p == NULL){
        fprintf(stderr, "Invalid bezier surface.\n");
        return;
    }

    p->zBuffer = flag;
}

// draws the Bezier curve, given in screen coordinates, into the image using the given color
// The function should be adaptive so that it uses an appropriate number of line segments to draw the curve.
// For example, if the bounding box of the control points is less than 10 pixels in the largest dimension,
// then it is reasonable to draw the lines between the control points as an approximation to the curve.
void bezierCurve_draw(BezierCurve *b, Image *src, Color c){
    if (b == NULL || src == NULL) {
        fprintf(stderr, "Invalid Bezier curve or Image.\n");
        return;
    }

    float largestDimension = getLargestDimension(b);

    // base case
    if (largestDimension <= 10) {
        // Draw lines between the control points
        for (int i = 0; i < 3; i++) {
            Line l;
            line_set(&l, b->p[i], b->p[i+1]);
            line_draw(&l, src, c);
        }
    }else{
        BezierCurve left, right;
        de_casteljau(b, &left, &right);
        bezierCurve_draw(&left, src, c);
        bezierCurve_draw(&right, src, c);
    }
}

// calculate the bounding box of the Bezier curve and return the largest dimension
float getLargestDimension(BezierCurve *b) {
    float minX = fminf(fminf(b->p[0].val[0], b->p[1].val[0]), fminf(b->p[2].val[0], b->p[3].val[0]));
    float maxX = fmaxf(fmaxf(b->p[0].val[0], b->p[1].val[0]), fmaxf(b->p[2].val[0], b->p[3].val[0]));
    float minY = fminf(fminf(b->p[0].val[1], b->p[1].val[1]), fminf(b->p[2].val[1], b->p[3].val[1]));
    float maxY = fmaxf(fmaxf(b->p[0].val[1], b->p[1].val[1]), fmaxf(b->p[2].val[1], b->p[3].val[1]));

    // Calculate dimensions
    float width = maxX - minX;
    float height = maxY - minY;

    return fmaxf(width, height);
}

void de_casteljau(BezierCurve *b, BezierCurve *left, BezierCurve *right){
    Point midpoints[3];

    // Calculate midpoints
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            midpoints[i].val[j] = (b->p[i].val[j] + b->p[i + 1].val[j]) / 2.0;
            midpoints[i].val[3] = 1;
        }
    }

    // Populate left curve
    point_copy(&(left->p[0]), &(b->p[0]));
    point_copy(&(left->p[1]), &(midpoints[0]));
    for (int j = 0; j < 3; j++) {
        left->p[2].val[j] = (midpoints[0].val[j] + midpoints[1].val[j]) / 2.0;
        left->p[2].val[3] = 1;
    }
    for (int j = 0; j < 3; j++) {
        left->p[3].val[j] = (left->p[2].val[j] + midpoints[2].val[j]) / 2.0;
        left->p[3].val[3] = 1;
    }

    // Populate right curve
    point_copy(&(right->p[3]), &(b->p[3]));
    point_copy(&(right->p[2]), &(midpoints[2]));
    for (int j = 0; j < 3; j++) {
        right->p[1].val[j] = (midpoints[2].val[j] + midpoints[1].val[j]) / 2.0;
        right->p[1].val[3] = 1;
    }
    for (int j = 0; j < 3; j++) {
        right->p[0].val[j] = (right->p[1].val[j] + midpoints[0].val[j]) / 2.0;
        right->p[0].val[3] = 1;
    }
}
