#include <stdio.h>
#include <stdlib.h>
#include "polyline.h"
#include "line.h"

// returns an allocated Polyline pointer initialized so that numVertex is 0 and vertex is NULL
Polyline *polyline_create(){
    Polyline *pl = (Polyline*)malloc(sizeof(Polyline));
    if(pl == NULL){
        fprintf(stderr, "Unable to allocate.");
    }

    pl -> zBuffer = 1;
    pl -> numVertex = 0;
    pl -> vertex = NULL;

    return pl;
}

// returns an allocated Polyline pointer with the vertex list initialized to the points in vlist
Polyline *polyline_createp(int numV, Point *vlist){
    Polyline *pl = (Polyline*)malloc(sizeof(Polyline));
    if(pl == NULL){
        fprintf(stderr, "Unable to allocate polyline memory.");
    }

    pl -> zBuffer = 1;
    pl -> numVertex = numV;
    pl->vertex = (Point*)malloc(numV * sizeof(Point));
    if (pl->vertex == NULL) {
        fprintf(stderr, "Unable to allocate vertex memory.\n");
    }
    for (int i = 0; i < numV; i++) {
        point_copy(&(pl->vertex[i]), &(vlist[i]));
    }

    return pl;
}

// free the internal data and the polyline pointer
void polyline_free(Polyline *p){
    if(p == NULL) return;

    free(p -> vertex);
    free(p);
}

// initialize the pre-existing Polyline to an empty Polyline
void polyline_init(Polyline *p){
    if(p == NULL){
        fprintf(stderr, "Unable to initialize. Invalid polyline.");
    }

    p -> zBuffer = 1;
    p -> numVertex = 0;
    p -> vertex = NULL;
}

// initialize the vertex list to the points in vlist. De-allocate/allocate the vertex list for p as necessary
void polyline_set(Polyline *p, int numV, Point *vlist){
    if(p == NULL || vlist == NULL){
        fprintf(stderr, "Unable to initialize. Invalid polyline or vertex.");
    }

    if(p -> vertex != NULL){
        free(p -> vertex);
    }

    p -> vertex = (Point*)malloc(numV * sizeof(Point));
    if(p -> vertex == NULL){
        fprintf(stderr, "Unable to allocate vertex memory.");
    }

    for (int i = 0; i < numV; i++) {
        point_copy(&(p->vertex[i]), &(vlist[i]));
    }
    p -> numVertex = numV;
}

// free the internal data for a polyline, if necessary, and set numVertex to 0 and vertex to NULL
void polyline_clear(Polyline *p){
    if(p == NULL){
        fprintf(stderr, "Invalid polyline.");
    }

    if(p -> vertex != NULL){
        free(p -> vertex);
    }
    
    p -> numVertex = 0;
    p -> vertex = NULL;
}

// set the z-buffer flag to the given value
void polyline_zBuffer(Polyline *p, int flag){
    if(p == NULL){
        fprintf(stderr, "Invalid polyline. ");
    }

    p -> zBuffer = flag;
}

// de-allocate/allocate space as necessary in the destination polyline data structure
// copy the vertex data from the src to the dst
void polyline_copy(Polyline *to, Polyline *from){
    if(to == NULL || from == NULL){
        fprintf(stderr, "Unable to copy. Invalid source or destination.");
    }

    if(to -> vertex != NULL){
        free(to -> vertex);
    }

    to -> vertex = (Point*)malloc(from -> numVertex * sizeof(Point));
    if(to -> vertex == NULL){
        fprintf(stderr, "Unable to allocate memory.");
    }

    for (int i = 0; i < from -> numVertex; i++) {
        point_copy(&(to->vertex[i]), &(from->vertex[i]));
    }
    to -> numVertex = from -> numVertex;
    to -> zBuffer = from -> zBuffer;
}

// print polyline data to the stream designed by the FILE pointer
void polyline_print(Polyline *p, FILE *fp){
    if(p == NULL || fp == NULL){
        fprintf(stderr, "Unable to print. Invalid polyline or file.");
    }

    fprintf(fp, "zBuffer: %d\n", p -> zBuffer);
    fprintf(fp, "number of vertices: %d\n", p -> numVertex);

    for(int i = 0; i < p -> numVertex; i++){
        fprintf(fp, "Vertex %d: (%f, %f, %f, %f)\n", i, 
                p -> vertex -> val[0], p -> vertex -> val[1], p -> vertex -> val[2], p -> vertex -> val[3]);
    }
}

// normalize the x and y values of each vertex by the homogeneous coordinate
void polyline_normalize(Polyline *p){
    if(p == NULL){
        fprintf(stderr, "Invalid polyline.");
    }

    for (int i = 0; i < p->numVertex; i++) {
        if(p->vertex[i].val[3] == 0){
            fprintf(stderr, "Invalid weight. Unable to normalize polyline.\n");
        }
        p -> vertex[i].val[0] /= p -> vertex[i].val[3];
        p -> vertex[i].val[1] /= p -> vertex[i].val[3];
    }
}

// draw the polyline using color c and the z-buffer
void polyline_draw(Polyline *p, Image *src, Color c){
    if(p == NULL || src == NULL){
        fprintf(stderr, "Unable to draw. Invalid polyline or image.");
    }

    for(int i = 0; i < p -> numVertex - 1; i++){
        Line l;
        l.a = p->vertex[i];
        l.b = p->vertex[i + 1];
        line_draw(&l, src, c);
    }
}