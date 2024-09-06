#include <stdlib.h>
#include "drawstate.h"

// create a new DrawState structure and initialize the fields
DrawState *drawstate_create(void){
    DrawState *s = (DrawState*)malloc(sizeof(DrawState));
    if(s == NULL){
        fprintf(stderr, "Unable to allocate memory for DrawState.\n");
        return NULL;
    }
    Color white, surface;
    color_set(&white, 1.0, 1.0, 1.0);
    color_set(&surface, 0.1, 0.1, 0.1);

    color_copy(&(s->color), &white);
    color_copy(&(s->bodyColor), &white);
    color_copy(&(s->surfaceColor), &surface);
    color_copy(&(s->flatColor), &white);
    s->surfaceCoeff = 0.0;
    s->zBufferFlag = 1;
    s->shade = ShadeFrame;
    return s;
}

// set the color field to c
void drawstate_setColor(DrawState *s, Color c){
    if(s == NULL){
        fprintf(stderr, "Invalid DrawState.\n");
        return;
    }

    color_copy(&(s->color), &c);
}

// set the body field to c
void drawstate_setBody(DrawState *s, Color c){
    if(s == NULL){
        fprintf(stderr, "Invalid DrawState.\n");
        return;
    }

    color_copy(&(s->bodyColor), &c);
}

// set the surface field to c
void drawstate_setSurface(DrawState *s, Color c){
    if(s == NULL){
        fprintf(stderr, "Invalid DrawState.\n");
        return;
    }

    color_copy(&(s->surfaceColor), &c);
}

// set the surfaceCoeff field to f
void drawstate_setSurfaceCoeff(DrawState *s, float f){
    if(s == NULL){
        fprintf(stderr, "Invalid DrawState.\n");
        return;
    }

    s->surfaceCoeff = f;
}

// copy the drawstate data
void drawstate_copy(DrawState *to, DrawState *from){
    if(to == NULL || from == NULL){
        fprintf(stderr, "Invalid DrawStates.\n");
        return;
    }

    color_copy(&(to->color), &(from->color));
    color_copy(&(to->flatColor), &(from->flatColor));
    color_copy(&(to->bodyColor), &(from->bodyColor));
    color_copy(&(to->surfaceColor), &(from->surfaceColor));
    to->shade = from->shade;
    to->surfaceCoeff = from->surfaceCoeff;
    to->zBufferFlag = from->zBufferFlag;
    point_copy(&(to->viewer), &(from->viewer));
}

void drawstate_print(DrawState *s) {
    if (s == NULL) {
        fprintf(stderr, "Invalid DrawState.\n");
        return;
    }

    printf("DrawState:\n");
    printf("Foreground Color: ");
    color_print(&(s->color));

    printf("Flat Color: ");
    color_print(&(s->flatColor));

    printf("Body Color: ");
    color_print(&(s->bodyColor));

    printf("Surface Color: ");
    color_print(&(s->surfaceColor));

    printf("Surface Coefficient: %.2f\n", s->surfaceCoeff);
    printf("Z-Buffer Flag: %d\n", s->zBufferFlag);

    printf("Viewer: ");
    point_print(&(s->viewer),stdout);
}
