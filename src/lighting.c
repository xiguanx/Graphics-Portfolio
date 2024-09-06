#include <stdlib.h>
#include <math.h>
#include "lighting.h"

// light functions
// initialize the light to default values
void light_init(Light *light){
    light->type = LightNone;
    light->color = (Color){{0.0, 0.0, 0.0}};
    light->direction = (Vector){{0.0, 0.0, 0.0, 1.0}};
    light->position = (Point){{0.0, 0.0, 0.0, 1.0}};
    light->cutoff = 0.0;
    light->sharpness = 0.0;
}

// copy the light information
void light_copy(Light *to, Light *from){
    to->type = from->type;
    color_copy(&(to->color), &(from->color));
    vector_copy(&(to->direction), &(from->direction));
    point_copy(&(to->position), &(from->position));
    to->cutoff = from->cutoff;
    to->sharpness = from->sharpness;
}

void light_set(Light *light, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness){
    if(light == NULL || c == NULL){
        fprintf(stderr, "Invalid light or color.\n");
    }

    light->type = type;
    color_copy(&(light->color), c);
    if(dir != NULL){
        vector_copy(&(light->direction), dir);
    }
    if(pos != NULL){
        point_copy(&(light->position), pos);
    }    
    light->cutoff = cutoff;
    light->sharpness = sharpness;
}

//lighting functions
// allocate and return a new lighting structure set to default values
Lighting *lighting_create(void){
    Lighting *lights = (Lighting*)malloc(sizeof(Lighting));
    if(lights == NULL){
        fprintf(stderr, "Failed to allocate memory for lighting.\n");
    }

    lighting_init(lights);

    return lights;
}

// delete the lighting structure
void lighting_delete(Lighting *lights){
    if(lights == NULL){
        fprintf(stderr, "Invalid lighting.\n");
    }

    free(lights);
}

// initialize the lighting sturcture to default values
void lighting_init(Lighting *l){
    if(l == NULL){
        fprintf(stderr, "Invalid lighting.\n");
    }
    for(int i = 0; i < 64; i++){
        light_init(&(l->light[i]));
    }
    l->nLights = 0;
}

// reset the lighting structure to 0 lights
void lighting_clear(Lighting *l){
    if(l == NULL){
        fprintf(stderr, "Invalid lighting.\n");
    }
    for(int i = 0; i < 64; i++){
        light_init(&(l->light[i]));
    }
    l->nLights = 0;
}

// add a new light to the lighting structure given the parameters, some of which may be NULL, depending upon the type
// Make sure you don't add more lights than max_lights
void lighting_add(Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness){
    if(l == NULL || l->nLights > 64){
        fprintf(stderr, "Invalid lighting. Max lights reached. \n");
    }

    Light *light = &(l->light[l->nLights]);
    light->type = type;
    color_copy(&(light->color), c);
    if (dir != NULL) {
        vector_copy(&(light->direction), dir);
    }
    if (pos != NULL) {
        point_copy(&(light->position), pos);
    }
    light->cutoff = cutoff;
    light->sharpness = sharpness;

    l->nLights++;
}

// caculate the proper color given the normal N, view vector V, 3D point P, body color Cb, surface color Cs,
// sharpness value s, the lighting, and whether the polygon is one-sided or two-sided. 
// Put the result in the Color c
void lighting_shading(Lighting *l, Vector *N, Vector *V, Point *p, Color *Cb, Color *Cs, float s, int oneSided, Color *c){
    if(l == NULL || p == NULL || Cb == NULL || Cs == NULL ||c == NULL){
        fprintf(stderr, "Invalid lighting or parameters.\n");
    }

    Vector L, H;
    float dotNL, dotNH, dotNV;
    Color result = {{0.0, 0.0, 0.0}};
    vector_normalize(N);
    vector_normalize(V);
    dotNV = vector_dot(N, V);

    for(int i = 0; i < l->nLights; i++){
        Light light = l->light[i];
        Color body, surface;

        switch(light.type){
            case LightNone:               
                break;
            case LightAmbient:{
                result.c[0] += light.color.c[0] * Cb->c[0];
                result.c[1] += light.color.c[1] * Cb->c[1];
                result.c[2] += light.color.c[2] * Cb->c[2];
                break;
            }
            case LightDirect:{
                vector_copy(&L, &(light.direction));
                vector_normalize(&L);
                dotNL = vector_dot(N, &L);
                if(oneSided && dotNL < 0) {
                    continue;
                }
                if( (dotNL < 0 && dotNV > 0) || (dotNL > 0 && dotNV < 0) ){
                    continue;
                }
                vector_add(&H, &L, V);
                vector_scale(&H, 0.5);
                vector_normalize(&H);
                dotNH = vector_dot(&H, N);
                if(dotNL < 0 && !oneSided) {
                    dotNL = -dotNL;
                    dotNH = -dotNH;
                }
                dotNH = pow(dotNH, s);

                // body reflection
                body.c[0] = dotNL * light.color.c[0] * Cb->c[0];
                body.c[1] = dotNL * light.color.c[1] * Cb->c[1];
                body.c[2] = dotNL * light.color.c[2] * Cb->c[2];
                // surface reflection
                surface.c[0] = dotNH * light.color.c[0] * Cs->c[0];
                surface.c[1] = dotNH * light.color.c[1] * Cs->c[1];
                surface.c[2] = dotNH * light.color.c[2] * Cs->c[2];
                //integrated
                result.c[0] += body.c[0] + surface.c[0];
                result.c[1] += body.c[1] + surface.c[1];
                result.c[2] += body.c[2] + surface.c[2];
                break;
            }
            case LightPoint:{
                vector_subtract(&L, &(light.position), p);
                vector_normalize(&L);

                dotNL = vector_dot(N, &L);
                if(oneSided && dotNL < 0) {
                    continue;
                }
                if( (dotNL < 0 && dotNV > 0) || (dotNL > 0 && dotNV < 0) ){
                    continue;
                }

                vector_add(&H, &L, V);
                vector_scale(&H, 0.5);
                vector_normalize(&H);
                dotNH = vector_dot(&H, N);
                if(dotNL < 0 && !oneSided) {
                    dotNL = -dotNL;
                    dotNH = -dotNH;
                }
                dotNH = pow(dotNH, s);

		color_print( Cb );
		color_print( Cs );

                // body reflection
                body.c[0] = dotNL * light.color.c[0] * Cb->c[0];
                body.c[1] = dotNL * light.color.c[1] * Cb->c[1];
                body.c[2] = dotNL * light.color.c[2] * Cb->c[2];
                // surface reflection
                surface.c[0] = dotNH * light.color.c[0] * Cs->c[0];
                surface.c[1] = dotNH * light.color.c[1] * Cs->c[1];
                surface.c[2] = dotNH * light.color.c[2] * Cs->c[2];
                //integrated

		color_print( &body );
		color_print( &surface );
		
                result.c[0] += body.c[0] + surface.c[0];
                result.c[1] += body.c[1] + surface.c[1];
                result.c[2] += body.c[2] + surface.c[2];

		printf("result: ");
		color_print( &result );
		
                break;
            }
            case LightSpot:{
                vector_subtract(&L, &(light.position), p);
                vector_normalize(&L);

                dotNL = vector_dot(N, &L);
                if(oneSided && dotNL < 0) {
                    continue;
                }
                if( (dotNL < 0 && dotNV > 0) || (dotNL > 0 && dotNV < 0) ){
                    continue;
                }

                // Check spotlight cutoff
                vector_scale(&L, -1);
                float dotDL = vector_dot(&(light.direction), &L);
                if(dotDL < cos(light.cutoff)) {
                    continue;
                }
                vector_add(&H, &L, V);
                vector_scale(&H, 0.5);
                vector_normalize(&H);
                dotNH = vector_dot(&H, N);
                if(dotNL < 0 && !oneSided) {
                    dotNL = -dotNL;
                    dotNH = -dotNH;
                }
                dotNH = pow(dotNH, s);
                dotDL = pow(dotDL, light.sharpness);

                // body reflection
                body.c[0] = dotNL * light.color.c[0] * Cb->c[0];
                body.c[1] = dotNL * light.color.c[1] * Cb->c[1];
                body.c[2] = dotNL * light.color.c[2] * Cb->c[2];
                // surface reflection
                surface.c[0] = dotNH * light.color.c[0] * Cs->c[0];
                surface.c[1] = dotNH * light.color.c[1] * Cs->c[1];
                surface.c[2] = dotNH * light.color.c[2] * Cs->c[2];
                //integrated
                result.c[0] += body.c[0] + surface.c[0];
                result.c[1] += body.c[1] + surface.c[1];
                result.c[2] += body.c[2] + surface.c[2];

                break;
            }
            default:
                break;
        }
    }

    color_copy(c, &result);
}

void lighting_copy(Lighting *to, Lighting *from){
    if (to == NULL || from == NULL) {
        return;
    }
    to->nLights = from->nLights;
    for(int i = 0; i < from->nLights; i++){
        light_copy(&to->light[i], &from->light[i]);
    }
}
