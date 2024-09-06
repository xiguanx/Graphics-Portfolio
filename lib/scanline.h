#ifndef SCANLINE_H
#define SCANLINE_H
#include "point.h"
#include "image.h"
#include "polygon.h"
#include "vector.h"
#include "list.h"

// define the struct here, because it is local to only this file
typedef struct tEdge {
	float x0, y0;                   /* start point for the edge */
	float x1, y1;                   /* end point for the edge */
	int yStart, yEnd;               /* start row and end row */
    float xIntersect, dxPerScan;    /* where the edge intersects the current scanline and how it changes */
	float zIntersect, dzPerScan;  // z-buffer
	Color cIntersect, dcPerScan; // Gouraud shading
	Point pIntersect, dpPerScan; // Phong shading
	Vector nIntersect, dnPerScan; // Phong shading surface normal
	Texture texture;
	float sIntersect, dsPerScan; // texture coordinate
	float tIntersect, dtPerScan;
    struct tEdge *next;
} Edge;

int compYStart( const void *a, const void *b );
int compXIntersect( const void *a, const void *b );
Edge *makeEdgeRec( Point start, Point end, Image *src, DrawState *ds, Color c0, Color c1);
LinkedList *setupEdgeList(  Polygon *p, Image *src, DrawState *ds);
void fillScan( int scan, LinkedList *active, Image *src, DrawState *ds, Lighting *lights);
int processEdgeList( LinkedList *edges, Image *src, DrawState *ds, Lighting *lights );
void processEdgeListGradient(LinkedList *edges, Image *src, Color startColor, Color endColor);
void drawScanlineGradient(LinkedList *active, Image *src, Color startColor, Color endColor, int scan);

#endif