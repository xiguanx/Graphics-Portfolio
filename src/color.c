#include <stdio.h>
#include <stdlib.h>
#include "color.h"

// copy the color data
void color_copy(Color *to, Color *from){
    if(to == NULL || from == NULL){
        fprintf(stderr,"unable to copy color.");
    }

    to -> c[0] = from -> c[0];
    to -> c[1] = from -> c[1];
    to -> c[2] = from -> c[2];
}

// setter
void color_set(Color *to, float r, float g, float b){
    if(to == NULL){
        fprintf(stderr, "Unable to set color.");
    }

    to -> c[0] = r;
    to -> c[1] = g;
    to -> c[2] = b;
}

void color_print(Color *c) {
    printf("Color: R=%.2f, G=%.2f, B=%.2f\n", c->c[0], c->c[1], c->c[2]);
}
