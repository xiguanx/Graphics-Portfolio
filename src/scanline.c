/*
  Bruce A. Maxwell
  Fall 2014

	Skeleton scanline fill algorithm
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "vector.h"
#include "polygon.h"
#include "scanline.h"


/********************
Scanline Fill Algorithm
********************/

/*
	This is a comparison function that returns a value < 0 if a < b, a
	value > 0 if a > b, and 0 if a = b.  It uses the yStart field of the
	Edge structure.  It is used to sort the overall edge list.
	find the vertical extrema
 */
int compYStart( const void *a, const void *b ) {
	Edge *ea = (Edge *)a;
	Edge *eb = (Edge *)b;

	return(ea->yStart - eb->yStart);
}


/*
	This is a comparison function that returns a value < 0 if a < b, a
	value > 0 if a > b, and 0 if a = b.  It uses the xIntersect field of the
	Edge structure.  It is used to sort the active edge list.
 */
int compXIntersect( const void *a, const void *b ) {
	Edge *ea = (Edge *)a;
	Edge *eb = (Edge *)b;

	if( ea->xIntersect < eb->xIntersect )
		return(-1);
	else if(ea->xIntersect > eb->xIntersect )
		return(1);

	return(0);
}

/*
	Allocates, creates, fills out, and returns an Edge structure given
	the inputs.

	Current inputs are just the start and end location in image space.
	Eventually, the points will be 3D and we'll add color and texture
	coordinates.
 */
Edge *makeEdgeRec( Point start, Point end, Image *src, DrawState *ds, Color c0, Color c1)
{
	Edge *edge;
	float dscan = end.val[1] - start.val[1];

	/******
				 Your code starts here
	******/

	// Check if the starting row is below the image or the end row is
	// above the image and skip the edge if either is true
	if (start.val[1] >= src->rows || end.val[1] < 0) {
		return NULL;
	}
	// allocate an edge structure and set the x0, y0, x1, y1 values
	edge = (Edge*)malloc(sizeof(Edge));
	if(edge == NULL){
		fprintf(stderr, "Failed to allocate memory for edge.\n");
		return NULL;
	}
	edge -> x0 = start.val[0];
	edge -> y0 = start.val[1];
	edge -> x1 = end.val[0];
	edge -> y1 = end.val[1];
	edge -> yStart = (int)(edge -> y0 + 0.5);
	edge -> yEnd = (int)(edge -> y1 + 0.5) - 1;

	// BAM put these calculations here
	float invZ0 = (start.val[2] != 0.0) ? 1.0 / start.val[2] : FLT_MAX; //BAM might want to set it to a big number if start.val[2] is zero
	float invZ1 = (end.val[2] != 0.0) ? 1.0 / end.val[2] : FLT_MAX;  // BAM same here

	if (edge->yEnd >= src->rows) {
    	edge->yEnd = src->rows - 1;
	}
	
	edge->dxPerScan = (edge->x1 - edge->x0) / dscan;
	edge->dzPerScan = (invZ1 - invZ0) / dscan; 	
	//edge->dpPerScan.val[0] = 
	//edge->dnPerScan = 

	// BAM I would suggest computing the adjustment once, because you will use it for both xIntersect and zIntersect
	float adjust = (edge->yStart - edge->y0 + 0.5);
	edge->xIntersect = edge->x0 + adjust * edge->dxPerScan;
	edge->zIntersect = invZ0 + adjust * edge->dzPerScan;

	// color interpolation
	if(ds->shade == ShadeFlat){
		for (int i = 0; i < 3; i++){
			edge->dcPerScan.c[i] = 0;
			edge->cIntersect.c[i] = c0.c[i];
		}
	}else if(ds->shade == ShadeGouraud){
		for (int i = 0; i < 3; i++) {
				edge->dcPerScan.c[i] = (c1.c[i] * invZ1 - c0.c[i] * invZ0) / dscan;
				edge->cIntersect.c[i] = c0.c[i] * invZ0 + adjust * edge->dcPerScan.c[i];
    		}
	}
    
	if(edge->y0 < 0){
		edge->xIntersect = edge->x0 + (-edge->y0) * edge->dxPerScan;
		edge->zIntersect = invZ0 + (-edge->y0) * edge->dzPerScan;
		edge->yStart = 0;
		edge->y0 = 0.0;
    	edge->x0 = edge->xIntersect;

		if(ds->shade == ShadeGouraud){
			for (int i = 0; i < 3; i++) {
            	edge->cIntersect.c[i] += edge->dcPerScan.c[i] * (-edge->y0);
        	}
		}
	}
	if (edge->dxPerScan < 0.0 && edge->xIntersect < edge->x1) {
		edge->xIntersect = edge->x1;
		edge->zIntersect = invZ1;
		// BAM might want to set zIntersect to edge->z1
	} else if (edge->dxPerScan > 0.0 && edge->xIntersect > edge->x1) {
		edge->xIntersect = edge->x1;
		edge->zIntersect = invZ1;
		// BAM might want to set zIntersect to edge->z1
	}

	return( edge );
}

/*
	Returns a list of all the edges in the polygon in sorted order by
	smallest row.
*/
LinkedList *setupEdgeList( Polygon *p, Image *src, DrawState *ds) {
	LinkedList *edges = NULL;
	Point v1, v2;
	Color c1, c2;
	int i;

	edges = ll_new();

	v1 = p->vertex[p->nVertex-1];
	c1 = p->color ? p->color[p->nVertex-1] : ds->color;

	for(i=0;i<p->nVertex;i++) {
		v2 = p->vertex[i];
		c2 = p->color ? p->color[i] : ds->color;
		// if it is not a horizontal line
		if( (int)(v1.val[1]+0.5) != (int)(v2.val[1]+0.5) ) {
			Edge *edge;
			if( v1.val[1] < v2.val[1] )
				edge = makeEdgeRec( v1, v2, src, ds, c1, c2);
			else
				edge = makeEdgeRec( v2, v1, src, ds, c2, c1);

			// insert the edge into the list of edges if it's not null
			if( edge )
				ll_insert( edges, edge, compYStart );
		}
		v1 = v2;
		c1 = c2;
	}

	if( ll_empty( edges ) ) {
		ll_delete( edges, NULL );
		edges = NULL;
	}

	return(edges);
}

/*
	Draw one scanline of a polygon given the scanline, the active edges,
	a DrawState, the image, and some Lights (for Phong shading only).
 */
void fillScan( int scan, LinkedList *active, Image *src, DrawState *ds, Lighting *lights) {
	Edge *p1, *p2;
	Color dcPerColumn;
	Color curColor = ds->color;
	float curs=0, curt=0, dsPerColumn=0, dtPerColumn=0;
	// loop over the list
	p1 = ll_head( active );
	while(p1) {
			// the edges have to come in pairs, draw from one to the next
		p2 = ll_next( active );
		if( !p2 ) {
			printf("bad bad bad (your edges are not coming in pairs)\n");
			break;
		}

			// if the xIntersect values are the same, don't draw anything.
			// Just go to the next pair.
		if( p2->xIntersect == p1->xIntersect ) {
			p1 = ll_next( active );
			continue;
		}

			/**** Your code goes here ****/

		// BAM you want to compute these here
		float dzPerColumn = (p2->zIntersect - p1->zIntersect) / (p2->xIntersect - p1->xIntersect);
		float curZ = p1->zIntersect;		
		switch(ds->shade) {
			case ShadeConstant:
				dzPerColumn = 0;
				curZ = 1;
				break;
			case ShadeFlat:
				curColor = p1->cIntersect;
				dcPerColumn.c[0] = 0;
				dcPerColumn.c[1] = 0;
				dcPerColumn.c[2] = 0;
				break;
			case ShadeDepth:
				curColor.c[0] = p1->zIntersect;
				dcPerColumn.c[0] = dzPerColumn;
				break;
			case ShadeGouraud:
				curColor = p1->cIntersect;
				for(int i=0; i<3; i++){
					dcPerColumn.c[i] = (p2->cIntersect.c[i] - p1->cIntersect.c[i]) / (p2->xIntersect - p1->xIntersect);
				}
				break;
			case ShadePhong:
			    break;
			default:
				break;
		}
		
		int startCol = (int)(p1->xIntersect + 0.5);
		if (startCol < 0) {
		  // BAM if you do this, you need to adjust curZ by adding -startCol * dzPerColumn
		  curZ += -startCol * dzPerColumn;
		  for (int i = 0; i < 3; i++) {
              curColor.c[i] += -startCol * dcPerColumn.c[i];
          }
		  curs += -startCol*dsPerColumn;
		  curt += -startCol*dtPerColumn;
		  startCol = 0;
		}
		
		int endCol = (int)(p2->xIntersect + 0.5);
		if (endCol >= src->cols) endCol = src->cols;  // BAM src->cols, not src->cols-1
		
		for (int x = startCol; x <= endCol; x++) {
		  if (ds->shade == ShadeConstant || curZ > image_getz(src, scan, x)){ // BAM or ds->shade == ShadeConstant
				FPixel pixel;
				switch(ds->shade){
					case ShadeConstant:
					case ShadeFlat:
						color_copy(&(pixel.c), &(curColor));
						break;
					case ShadeDepth:
					  { // BAM put brackets around a case when you have a variable
						float depthV = 1.0f - (1.0/curZ); 
						pixel.c.c[0] = curColor.c[0] * depthV;
						pixel.c.c[1] = curColor.c[1] * depthV;
						pixel.c.c[2] = curColor.c[2] * depthV;
					  }
						break;
					case ShadeGouraud:
						pixel.c.c[0] = curColor.c[0] / curZ; // BAM divide by 1/z, not multiply
						pixel.c.c[1] = curColor.c[1] / curZ;
						pixel.c.c[2] = curColor.c[2] / curZ;
						printf("Writing: ");
						color_print(&pixel.c);
						break;
					default:
					    break;
				}
				pixel.z = curZ;
				image_setf(src, scan, x, pixel);
				image_setz(src, scan, x, curZ);
			}
			curZ += dzPerColumn;
			for (int i = 0; i < 3; i++) {
                curColor.c[i] += dcPerColumn.c[i];
            }
		}
		// move ahead to the next pair of edges
		p1 = ll_next( active );
	}
}

/* 
	 Process the edge list, assumes the edges list has at least one entry
*/
int processEdgeList( LinkedList *edges, Image *src, DrawState *ds, Lighting *lights ) {
	LinkedList *active = ll_new( );
	LinkedList *tmplist = ll_new( );
	LinkedList *transfer = NULL;
	Edge *current;
	Edge *tedge;
	int scan = 0;

	current = ll_head( edges );

	for(scan = current->yStart;scan < src->rows;scan++ ) {
		while( current != NULL && current->yStart == scan ) {
			ll_insert( active, current, compXIntersect );
			current = ll_next( edges );
		}
		
		if( ll_empty(active) ) {
			break;
		}

		fillScan(scan, active, src, ds, lights);
		for( tedge = ll_pop( active ); tedge != NULL; tedge = ll_pop( active ) ) {
			if( tedge->yEnd > scan ) {
				//float a = 1.0;

				if( tedge->dxPerScan < 0.0 && tedge->xIntersect < tedge->x1 ) {
					tedge->xIntersect = tedge->x1;
				}
				else if( tedge->dxPerScan > 0.0 && tedge->xIntersect > tedge->x1 ) {
					tedge->xIntersect = tedge->x1;
				}

				tedge->xIntersect += tedge->dxPerScan;
				tedge->zIntersect += tedge->dzPerScan;
				tedge->cIntersect.c[0] += tedge->dcPerScan.c[0];
				tedge->cIntersect.c[1] += tedge->dcPerScan.c[1];
				tedge->cIntersect.c[2] += tedge->dcPerScan.c[2];

				ll_insert( tmplist, tedge, compXIntersect );
			}
		}
		transfer = active;
		active = tmplist;
		tmplist = transfer;
	}

	ll_delete(active, NULL);
	ll_delete(tmplist, NULL);

	return(0);
}

// Process edge list with gradient colors
void processEdgeListGradient(LinkedList *edges, Image *src, Color startColor, Color endColor) {
    LinkedList *active = NULL;
    LinkedList *tmplist = NULL;
    LinkedList *transfer = NULL;
    Edge *current;
    Edge *tedge;
    int scan = 0;

    active = ll_new();
    tmplist = ll_new();

    current = ll_head(edges);

    for (scan = current->yStart; scan < src->rows; scan++) {
        // Grab all edges starting on this row
        while (current != NULL && current->yStart == scan) {
            ll_insert(active, current, compXIntersect);
            current = ll_next(edges);
        }

        if (ll_empty(active)) {
            break;
        }

        for (tedge = ll_pop(active); tedge != NULL; tedge = ll_pop(active)) {
            if (tedge->yEnd > scan) {
                tedge->xIntersect += tedge->dxPerScan;

                if (tedge->dxPerScan < 0.0 && tedge->xIntersect < tedge->x1) {
                    tedge->xIntersect = tedge->x1;
                } else if (tedge->dxPerScan > 0.0 && tedge->xIntersect > tedge->x1) {
                    tedge->xIntersect = tedge->x1;
                }

                ll_insert(tmplist, tedge, compXIntersect);
            }
        }

        // Transfer active list
        transfer = active;
        active = tmplist;
        tmplist = transfer;

        // Draw scanline with gradient color
        drawScanlineGradient(active, src, startColor, endColor, scan);
    }

    ll_delete(active, NULL);
    ll_delete(tmplist, NULL);
}

// Linear interpolation function
static float lerp(float a, float b, float t) {
    return a * (1.0f - t) + b * t;
}

// Draw scanline with gradient color
void drawScanlineGradient(LinkedList *active, Image *src, Color startColor, Color endColor, int scan) {
    Edge *e0, *e1;
    int x0, x1;
    float t;
    Color pixelColor;

    // Initialize the list traversal
    e0 = ll_head(active);

    // Iterate through pairs of edges on the active list
    while (e0) {
        e1 = ll_next(active);
        if (!e1) {
            printf("Error: edges are not in pairs.\n");
            break;
        }

        // Calculate starting and ending x coordinates for the current scanline
        x0 = (int)(e0->xIntersect);
        x1 = (int)(e1->xIntersect);

        // Ensure x0 and x1 are within image boundaries
        if (x0 < 0) x0 = 0;
        if (x1 >= src->cols) x1 = src->cols - 1;

        // Calculate the color gradient along the scanline and fill the pixels
        for (int x = x0; x <= x1; x++) {
            // Calculate interpolation factor
            t = (float)(x - x0) / (float)(x1 - x0);

            // Interpolate color between startColor and endColor
            pixelColor.c[0] = (unsigned char)lerp(startColor.c[0], endColor.c[0], t);
            pixelColor.c[1] = (unsigned char)lerp(startColor.c[1], endColor.c[1], t);
            pixelColor.c[2] = (unsigned char)lerp(startColor.c[2], endColor.c[2], t);

            // Set pixel color in the image
            FPixel pixel;
            pixel.c = pixelColor;
            image_setf(src, scan, x, pixel);
        }

        // Move to the next pair of edges
        e0 = ll_next(active);
    }
}

/****************************************
End Scanline Fill
*****************************************/
