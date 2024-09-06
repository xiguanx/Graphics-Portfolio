#include <stdlib.h>
#include "image.h"
#include "line.h"

// initialize a 2D line
void line_set2D(Line *l, double x0, double y0, double x1, double y1){
    if(l == NULL){
        fprintf(stderr, "Unable to initialize the line.");
        return;
    }

    point_set2D(&(l->a), x0, y0);
    point_set2D(&(l->b), x1, y1);
    l -> zBuffer = 1;
}

// initialize a line to ta and tb
void line_set(Line *l, Point ta, Point tb){
    if(l == NULL){
        fprintf(stderr, "Unable to initialize the line.");
        return;
    }

    point_copy(&(l->a), &(ta));
    point_copy(&(l->b), &(tb));
    l -> zBuffer = 1;
}

// set the z-buffer flag to the given value
void line_zBuffer(Line *l, int flag){
    if(l == NULL){
        fprintf(stderr, "Invalid line.");
        return;
    }

    l -> zBuffer = flag;
}

// normalize the x and y values of the endpoints by their homogeneous coordinate
void line_normalize(Line *l){
    if(l == NULL){
        fprintf(stderr, "Invalid line.\n");
        return;
    }

    if(l->b.val[3] == 0){
        fprintf(stderr, "Invalid weight.Unable to normalize line.\n");
        return;
    }
    
    l -> a.val[0] = l -> a.val[0] / l -> a.val[3];
    l -> a.val[1] = l -> a.val[1] / l -> a.val[3];
    l -> a.val[3] = 1;

    l -> b.val[0] = l -> b.val[0] / l -> b.val[3];
    l -> b.val[1] = l -> b.val[1] / l -> b.val[3];
    l -> b.val[3] = 1;
}

// copy the line data structure
void line_copy(Line *to, Line *from){
    if(to == NULL || from == NULL){
        fprintf(stderr, "Unable to copy. Invalid lines.");
    }

    to -> a = from -> a;
    to -> b = from -> b;
    to -> zBuffer = from -> zBuffer; 
}

// draw the line into src using color c and the z-buffer
void line_draw(Line *l, Image *src, Color c){
    if(l == NULL || src == NULL){
        fprintf(stderr, "Unable to draw. Invalid line or image.");
    }

    int x0 = (int)l -> a.val[0];
    int y0 = (int)l -> a.val[1];
    int x1 = (int)l -> b.val[0];
    int y1 = (int)l -> b.val[1];

    double z0 = l->a.val[2];
    double z1 = l->b.val[2];

    double inv_z0 = 1.0 / z0;
    double inv_z1 = 1.0 / z1;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    double delta_inv_z;
    if (dx > dy) {
        delta_inv_z = (inv_z1 - inv_z0) / dx;
    } else {
        delta_inv_z = (inv_z1 - inv_z0) / dy;
    }

    // Initial 1/z value
    double inv_z = inv_z0;

    while (x0 != x1 || y0 != y1) {
        // Set the pixel at (x0, y0) considering the z-buffer
        if (l->zBuffer) {
            if (inv_z > image_getz(src, y0, x0)) {
                FPixel val = {c, 1.0, 1.0};
                image_setf(src, y0, x0, val);
                image_setz(src, y0, x0, inv_z);
            }
        } else {
            FPixel val = {c, 1.0, 1.0};
            image_setf(src, y0, x0, val);
        }

        // Update the current 1/z value
        inv_z += delta_inv_z;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }

    // Final pixel
    if (l->zBuffer) {
        if (inv_z > image_getz(src, y1, x1)) {
            FPixel val = {c, 1.0, 1.0};
            image_setf(src, y1, x1, val);
            image_setz(src, y1, x1, inv_z);
        }
    } else {
        FPixel val = {c, 1.0, 1.0};
        image_setf(src, y1, x1, val);
    }
}

// draw dash line with a given length
void line_drawDash(Line *l, Image *src, Color c, int length){
    if(l == NULL || src == NULL){
        fprintf(stderr, "Unable to draw. Invalid line or image.");
    }

    int x0 = (int)l -> a.val[0];
    int y0 = (int)l -> a.val[1];
    int x1 = (int)l -> b.val[0];
    int y1 = (int)l -> b.val[1];

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;
    int count = 0;
    int draw = 1; // draw or not draw flag

    while (x0 != x1 || y0 != y1) {
        if(draw){
            FPixel val = {c, 1.0, 1.0};
            image_setf(src, y0, x0, val);
        }
        count++;
        if(count >= length){
            count = 0;
            draw = !draw;
        }

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}