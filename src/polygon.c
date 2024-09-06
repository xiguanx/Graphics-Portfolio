#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "polygon.h"
#include "line.h"
#include "scanline.h"

// return an allocated polygon pointer initialized so that num Vertex is 0 and vertex is NULL
Polygon *polygon_create(){
    Polygon *p = (Polygon*)malloc(sizeof(Polygon));
    if(p == NULL){
        fprintf(stderr, "Failed to allocate memory for polygon.\n");
        return NULL;
    }

    p -> oneSided = 1;
    p -> nVertex = 0;
    p -> vertex = NULL;
    p -> color = NULL;
    p -> normal = NULL;
    p -> zBuffer = 1;

    return p;
}

// return an allocated polygon pointer with the vertex list initialized to a copy of the points in vlist
Polygon *polygon_createp(int numV, Point *vlist){
    Polygon *p = (Polygon*)malloc(sizeof(Polygon));
    if(p == NULL){
        fprintf(stderr, "Failed to allocate memory for polygon.\n");
        return NULL;
    }

    if(vlist == NULL){
        fprintf(stderr, "Invalid vertex list.\n");
        free(p);
        return NULL;
    }
    
    p->vertex = (Point*)malloc(numV * sizeof(Point));
    if(p->vertex == NULL){
        fprintf(stderr, "Failed to allocate memory for vertices.\n");
        free(p);
        return NULL;
    }
    for(int i = 0; i < numV; i++){
        point_copy(&(p->vertex[i]), &(vlist[i]));
    }

    p->color = (Color*)malloc(numV*sizeof(Color));
    if(p->color == NULL){
        fprintf(stderr, "Failed to allocate memory for colors.\n");
        free(p);
        return NULL;
    }
    for(int i = 0; i < numV; i++){
        p->color[i].c[0] = 0.0;
        p->color[i].c[1] = 0.0;
        p->color[i].c[2] = 0.0;
    }

    p -> oneSided = 1;
    p -> nVertex = numV;
    p -> normal = NULL;
    p -> zBuffer = 1;

    return p;
}

// free the internal data for a polygon and the polygon pointer
void polygon_free(Polygon *p){
    if(p == NULL){
        fprintf(stderr, "Unable to free memory. Invalid polygon.\n");
        return;
    }

    if(p->vertex != NULL){
        free(p->vertex);
    }
    if(p->color != NULL){
        free(p->color);
    }
    if(p->normal != NULL){
        free(p->normal);
    }
    
    free(p);
}

// the functions init, set and clear work on a pre-existing Polygon data structure and manage only the memory required for the vertex list
// initializes the existing polygon to an empty polygon
void polygon_init(Polygon *p){
    if(p == NULL){
        fprintf(stderr, "Unable to initialize. Invalid polygon.\n");
        return;
    }

    p -> oneSided = 0; // BAM set this to default to 0
    p -> nVertex = 0;
    p -> vertex = NULL;
    p -> color = NULL;
    p -> normal = NULL;
    p -> zBuffer = 1;
}

// initialize the vertex array to the points in vlist
void polygon_set(Polygon *p, int numV, Point *vlist){
    if(p == NULL || vlist == NULL){
        fprintf(stderr, "Invalid polygon or vertices.\n");
        return;
    }

    if(p->vertex != NULL){
        free(p->vertex);
    }
    
    p->vertex = (Point*)malloc((numV) * sizeof(Point));
    if(p -> vertex == NULL){
        fprintf(stderr, "Failed to allocate memory for vertices.\n");
        return;
    }

    for(int i = 0; i < numV; i++){
        point_copy(&(p->vertex[i]), &(vlist[i]));
    }

    p -> nVertex = numV;
}

// free the internal data and reset the fields
void polygon_clear(Polygon *p){
    if(p == NULL){
        fprintf(stderr, "Invalid polygon.\n");
        return;
    }

    if(p -> vertex != NULL){
        free(p -> vertex);
        p->vertex = NULL;
    }
    if(p -> color != NULL){
        free(p -> color);
        p->color = NULL;
    }
    if (p->normal != NULL) {
        free(p->normal);
        p->normal = NULL;
    }

    p->nVertex = 0;
}

// set the oneSided to the value
void polygon_setSided(Polygon *p, int oneSided){
    if(p == NULL){
        fprintf(stderr, "Invalid polygon.\n");
        return;
    }

    p->oneSided = oneSided;
}

// initialize the color array to the colors in clist
void polygon_setColors(Polygon *p, int numV, Color *clist){
    if(p == NULL || clist == NULL){
        fprintf(stderr, "Invalid polygon or color list.\n");
        return;
    }

    if(p->color != NULL){
        free(p -> color);
    }

    p->color = (Color*)malloc(numV * sizeof(Color));
    if(p -> color == NULL){
        fprintf(stderr, "Failed to allocate memory for color array.\n");
        return;
    }

    for(int i = 0; i < numV; i++){
        color_copy(&(p->color[i]), &(clist[i]));
    }
}

// initializes the normal array to the vectors in nlist
void polygon_setNormals(Polygon *p, int numV, Vector *nlist){
    if(p == NULL || nlist == NULL){
        fprintf(stderr, "Invalid polygon or normal list.\n");
        return;
    }

    if(p->normal != NULL){
        free(p->normal);
    }


    p->normal = (Vector*)malloc(numV * sizeof(Vector));
    if(p->normal == NULL){
        fprintf(stderr, "Failed to allocate memory for normal list.\n");
        return;
    }

    for(int i = 0; i < numV; i++){
        vector_copy(&(p->normal[i]), &(nlist[i]));
    }
}

// initializes the vertex list to the points in vlist, the colors to the colors in clist, the normals to the vectors in nlist
// and the zBuffer and oneSided flags to their respectively values
void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist, Vector *nlist, int zBuffer, int oneSided){
    if(p != NULL){
        polygon_clear(p);
    }
    polygon_set(p, numV, vlist);
    polygon_setColors(p, numV, clist);
    polygon_setNormals(p, numV, nlist);
    polygon_setSided(p, oneSided);
    polygon_zBuffer(p, zBuffer);
}

// set the z-Buffer flag to the given value
void polygon_zBuffer(Polygon *p, int flag){
    if(p == NULL){
        fprintf(stderr, "Invalid polygon or color list.\n");
        return;
    }

    p -> zBuffer = flag;
}

// de-allocate/allocate space and copy the vertex and color data from one polygon to the other
void polygon_copy(Polygon *to, Polygon *from){
    if(to == NULL || from == NULL){
        fprintf(stderr, "Unable to copy. Invalid polygons.\n");
        return;
    }

    polygon_clear(to);

    if(from -> vertex != NULL){
        to -> vertex = (Point*)malloc(from -> nVertex * sizeof(Point));
        if(to -> vertex == NULL){
            fprintf(stderr, "Failed to allocate memory for vertices.\n");
            return;
        }
        for(int i = 0; i < from -> nVertex; i++){
            point_copy(&(to->vertex[i]), &(from->vertex[i]));
        }
    }

    if(from -> color != NULL){
        to -> color = (Color*)malloc(from -> nVertex * sizeof(Color));
        if(to -> color == NULL){
            fprintf(stderr, "Failed to allocate memory for color list.\n");
            return;
        }
        for(int i = 0; i < from -> nVertex; i++){
            to -> color[i] = from -> color[i];
        }
    }

    if(from -> normal != NULL){
        to -> normal = (Vector*)malloc(from -> nVertex * sizeof(Vector));
        if(to -> normal == NULL){
            fprintf(stderr, "Failed to allocate memory for normal list.\n");
            return;
        }
        for(int i = 0; i < from -> nVertex; i++){
            vector_copy(&(to->normal[i]), &(from->normal[i]));
        }
    }

    to->oneSided = from->oneSided;
    to->nVertex = from->nVertex;
    to->zBuffer = from->zBuffer;
}

// print polygon data to the stream designated by the FILE pointer
void polygon_print(Polygon *p, FILE *fp){
    if(p == NULL || fp == NULL){
        fprintf(stderr, "Invalid polygon or file stream.\n");
        return;
    }

    fprintf(fp, "One-sided: %d\n", p->oneSided);
    fprintf(fp, "Number of Vertices: %d\n", p->nVertex);
    fprintf(fp, "Use Z-buffer: %d\n", p->zBuffer);

    // Print vertices
    if(p->vertex != NULL){
        for(int i = 0; i < p->nVertex; i++){
            fprintf(fp, "%d\t%f\t%f\t%f\t%f\n", i, p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i].val[2], p->vertex[i].val[3]);
        }
    } else {
        fprintf(fp, "No vertex data available.\n");
    }

    // Print color
    if(p->color != NULL){
        for(int i = 0; i < p->nVertex; i++){
            fprintf(fp, "%d\t%f\t%f\t%f\n", i, p->color[i].c[0], p->color[i].c[1], p->color[i].c[2]);
        }
    } else {
        fprintf(fp, "No color data available.\n");
    }

     // Print normal
    if(p->normal != NULL){
        for(int i = 0; i < p->nVertex; i++){
            fprintf(fp, "%d\t%f\t%f\t%f\n", i, p->normal[i].val[0], p->normal[i].val[1], p->normal[i].val[2]);
        }
    } else {
        fprintf(fp, "No normal data available.\n");
    }
}

// normalize the x and y values of each vertex by the homogeneous coord
void polygon_normalize(Polygon *p){
    if(p == NULL){
        fprintf(stderr, "Invalid polygon.\n");
        return;
    }

    for(int i = 0; i < p->nVertex; i++){
        if(p->vertex[i].val[3] == 0){
            fprintf(stderr, "Invalid weight. Unable to normalize polygon.\n");
            return;
        }
        p -> vertex[i].val[0] = p -> vertex[i].val[0] / p -> vertex[i].val[3];
        p -> vertex[i].val[1] = p -> vertex[i].val[1] / p -> vertex[i].val[3];
        p -> vertex[i].val[3] = 1;
    }
}

// draw the outline of the polygon using color c
void polygon_draw(Polygon *p, Image *src, Color c){
    if(p == NULL || src == NULL){
        fprintf(stderr, "Unable to draw. Invalid polygon or image.\n");
    }

    if(p->nVertex < 2 || p->vertex == NULL){
        fprintf(stderr, "Unable to draw. Not sufficient vertices.\n");
    }

    for(int i = 0; i < p -> nVertex - 1; i++){
        Line l;
        l.a = p->vertex[i];
        l.b = p->vertex[i + 1];
        line_draw(&l, src, c);
    }

    // draw a line from the last vertex to the first vertex
    Line lc;
    lc.a = p->vertex[p->nVertex - 1];
    lc.b = p->vertex[0];
    line_draw(&lc, src, c);
}

/*
	Draws a filled polygon of the specified color into the image src.
 */
void polygon_drawFill(Polygon *p, Image *src, Color c ) {
	DrawState *ds = drawstate_create();
    drawstate_setColor(ds, c);
    ds->shade = ShadeConstant;

    polygon_drawShade(p, src, ds, NULL);
}

// Helper function to calculate the barycentric coordinates
static void barycentric(Point *p, Point *a, Point *b, Point *c, float *u, float *v, float *w) {
    float denom = (b->val[1] - c->val[1]) * (a->val[0] - c->val[0]) + (c->val[0] - b->val[0]) * (a->val[1] - c->val[1]);
    *u = ((b->val[1] - c->val[1]) * (p->val[0] - c->val[0]) + (c->val[0] - b->val[0]) * (p->val[1] - c->val[1])) / denom;
    *v = ((c->val[1] - a->val[1]) * (p->val[0] - c->val[0]) + (a->val[0] - c->val[0]) * (p->val[1] - c->val[1])) / denom;
    *w = 1.0f - *u - *v;
}

// draw the filled polygon using color c with the barycentric coordinates algo
void polygon_drawFillB(Polygon *p, Image *src, Color c){
    if (p == NULL || src == NULL) {
        fprintf(stderr, "Unable to draw. Invalid polygon or image.\n");
        return;
    }

    if (p->nVertex < 3 || p->vertex == NULL) {
        fprintf(stderr, "Unable to draw. Not sufficient vertices.\n");
        return;
    }

    // Find the bounding box of the polygon
    float minX = INFINITY, minY = INFINITY;
    float maxX = -INFINITY, maxY = -INFINITY;

    for (int i = 0; i < p->nVertex; i++) {
        if (p->vertex[i].val[0] < minX) minX = p->vertex[i].val[0];
        if (p->vertex[i].val[0] > maxX) maxX = p->vertex[i].val[0];
        if (p->vertex[i].val[1] < minY) minY = p->vertex[i].val[1];
        if (p->vertex[i].val[1] > maxY) maxY = p->vertex[i].val[1];
    }

    // Clamp bounding box to the image dimensions
    minX = fmax(0, minX);
    minY = fmax(0, minY);
    maxX = fmin(src->cols - 1, maxX);
    maxY = fmin(src->rows - 1, maxY);

    // Iterate over the bounding box
    Point pt;
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            pt.val[0] = x;
            pt.val[1] = y;
            pt.val[2] = 0;
            pt.val[3] = 1;

            // Check if the point (x, y) is inside the polygon using barycentric coordinates
            float u, v, w;
            int inside = 1;
            for (int i = 1; i < p->nVertex - 1; i++) {
                barycentric(&pt, &p->vertex[0], &p->vertex[i], &p->vertex[i + 1], &u, &v, &w);
                if (u < 0 || v < 0 || w < 0) {
                    inside = 0;
                    break;
                }
            }

            if (inside) {
                FPixel pixel;
                color_copy(&(pixel.c), &c);
                image_setf(src, y, x, pixel);
            }
        }
    }
}

// draw the filled polygon using the given DrawState
// the shade field of the DrawState determines how the polygon should be rendered
// The lighting parameter should be NULL unless you are doing Phong shading
// shadeframe: draw only the outline of the polygon using the drawstate color field(call polygon_draw)
// shaeconstant: fill the polygon with the draw state color field(call polygon_drawFill)
// shadedepth: fill the polygon based on the depth value, which should be in the range[0, 1] as 1 is the back clip plane in the canonical view space 
void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *light){
    if(ds->shade == ShadeFrame){
        polygon_draw(p, src, ds->color);
    }else {
        LinkedList *edges = NULL;
        edges = setupEdgeList( p, src, ds);
        if(!edges)
            return;       
        processEdgeList( edges, src, ds, light);

        ll_delete( edges, (void (*)(const void *))free );

    }
}

void polygon_shade(Polygon *p, DrawState *ds, Lighting *light){
    Vector v;
    if (p == NULL || ds == NULL || light == NULL) {
        fprintf(stderr, "Invalid polygon, drawstate, or lighting.\n");
        return;
    }

    if (p->color == NULL) {
        p->color = (Color*)malloc(p->nVertex * sizeof(Color));
    }
    // Calculate the color at each vertex
    for (int i = 0; i < p->nVertex; i++) {
        v.val[0] = ds->viewer.val[0] - p->vertex[i].val[0];
        v.val[1] = ds->viewer.val[1] - p->vertex[i].val[1];
        v.val[2] = ds->viewer.val[2] - p->vertex[i].val[2];
        lighting_shading(light, &(p->normal[i]), &v, &(p->vertex[i]), &(ds->bodyColor), &(ds->surfaceColor), ds->surfaceCoeff, p->oneSided, &(p->color[i]));    
    }
}

/*
// Allow the user to specify a gradient across a polygon
void polygon_drawFillGradient(Polygon *p, Image *src, Color startColor, Color endColor){
    if (p == NULL || src == NULL) {
        fprintf(stderr, "Unable to draw. Invalid polygon or image.\n");
        return;
    }

    if (p->nVertex < 3 || p->vertex == NULL) {
        fprintf(stderr, "Unable to draw. Not sufficient vertices.\n");
        return;
    }

    LinkedList *edges = NULL;

    // Set up the edge list
    edges = setupEdgeList(p, src, ds);
    if (!edges)
        return;

    // Process the edge list with gradient colors
    processEdgeListGradient(edges, src, startColor, endColor);

    // Clean up
    ll_delete(edges, (void (*)(const void *))free);
}
*/
