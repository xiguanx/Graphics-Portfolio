#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "module.h"

#define PI 3.14159265
#define NUM_FRAMES 36

int main(int argc, char *argv[]) {
    Image *src;
    Matrix VTM;
    Matrix GTM;
    Module *scene, *cuboid1, *cuboid2, *prism;
    int rows = 600;
    int cols = 800;
    Color White;
    Color Grey;
    Color DkGrey;
    DrawState *ds;
    View3D view;
    Lighting *light;

    color_set(&White, 1.0, 1.0, 1.0);
    color_set(&Grey, 0.6, 0.62, 0.64);
    color_set(&DkGrey, 0.1, 0.1, 0.1);

    scene = module_create();
    matrix_identity(&GTM);

    // vertical cuboid
    cuboid1 = module_create();
    module_scale(cuboid1, 1, 5.5, 1);
    module_color(cuboid1, &White);
    module_bodyColor(cuboid1, &White);
    module_surfaceColor(cuboid1, &DkGrey);
    module_cube(cuboid1, 1);
    module_module(scene, cuboid1);

    // horizontal cuboid
    cuboid2 = module_create();
    module_scale(cuboid2, 6.5, 1, 1);
    module_translate(cuboid2, -3.75, -2.2, 0); // position it on top of the first cuboid
    module_color(cuboid2, &White);
    module_bodyColor(cuboid2, &White);
    module_surfaceColor(cuboid2, &DkGrey);
    module_cube(cuboid2, 1);
    module_module(scene, cuboid2);

    prism = module_create();
    module_scale(prism, 3.5, 1, 1);
    module_rotateY(prism, 0, 1);
    module_translate(prism, 0, 2.26, -2.26);
    module_color(prism, &White);
    module_bodyColor(prism, &White);
    module_surfaceColor(prism, &DkGrey);
    module_prism(prism);
    module_module(scene, prism);

    ds = drawstate_create();
    ds->shade = ShadeGouraud;

    // Setup the view
    view.d = 2.0;
    view.du = 1.6;
    view.dv = 0.9;
    view.f = 0.0;
    view.b = 30;
    view.screenx = cols;
    view.screeny = rows;

    float radius = 15.0;
    float angle_step = 360.0 / NUM_FRAMES;

    for(int frame = 0; frame < NUM_FRAMES; frame++) {
        float angle = frame * angle_step * (PI / 180.0); // Convert degrees to radians

        // VRP based on the angle
        point_set3D(&(view.vrp), radius * sin(angle), 5, radius * cos(angle));
        vector_set(&(view.vpn), -radius * sin(angle), -5, -radius * cos(angle));
        vector_set(&(view.vup), 0.0, 1.0, 0.0);

        matrix_setView3D(&VTM, &view);
        // light source
        light = lighting_create();
        lighting_add(light, LightPoint, &White, NULL, &(view.vrp), 0, 0);

        src = image_create(rows, cols);
        module_draw(scene, &VTM, &GTM, ds, light, src);

        char filename[50];
        sprintf(filename, "frame_%02d.ppm", frame);
        image_write(src, filename);

        image_free(src);
    }

    module_delete(cuboid1);
    module_delete(cuboid2);
    module_delete(prism);
    module_delete(scene);

    return 0;
}