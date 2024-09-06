#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "module.h"

int main(int argc, char *argv[]) {
    Image *src;
    Matrix VTM;
    Matrix GTM;
    Module *cube, *cylinder, *cone, *sphere, *scene;
    int rows = 360;
    int cols = 640;

    Color Green = {{0.0, 1.0, 0.0}};
    Color Red = {{1.0, 0.0, 0.0}};
    Color Blue = {{0.0, 0.0, 1.0}};
    Color White = {{1.0, 1.0, 1.0}};
    Color DkGrey = {{0.1, 0.1, 0.1}};

    DrawState *ds;
    View3D view;
    Lighting *light;

    src = image_create(rows, cols);

    matrix_identity(&GTM);
    matrix_identity(&VTM);

    point_set3D(&(view.vrp), 5, 5, -7.0);
    vector_set(&(view.vpn), -5, -5, 7);
    vector_set(&(view.vup), 0.0, 1.0, 0.0);
    view.d = 2.0;
    view.du = 1.6;
    view.dv = 0.9;
    view.f = 0.0;
    view.b = 15;
    view.screenx = cols;
    view.screeny = rows;
    matrix_setView3D(&VTM, &view);

    // Create cylinder module
    cylinder = module_create();
    module_scale(cylinder, 1, 2, 1);
    module_translate(cylinder, -3, 0, 0);
    module_color(cylinder, &Green);
    module_bodyColor( cylinder, &Green );
    module_surfaceColor( cylinder, &DkGrey );
    module_cylinder(cylinder, 20);

    // Create cone module
    cone = module_create();
    module_scale(cone, 1, 2, 1);
    module_translate(cone, 0, 0, 0);
    module_color(cone, &Red);
    module_bodyColor( cone, &Red );
    module_surfaceColor( cone, &DkGrey );
    module_cone(cone, 20);

    // Create cube module
    sphere = module_create();
    module_scale(sphere, 1.5, 1.5, 1.5);
    module_translate(sphere, 3, 0, 0);
    module_color(sphere, &Blue);
    module_bodyColor( sphere, &Blue );
    module_surfaceColor( sphere, &DkGrey );
    module_sphere(sphere, 15, 15);

    cube = module_create();
    module_scale(cube, 1.0, 1.0, 1.0);
    module_translate(cube, 0, 2.5, 0); // You can adjust the position as needed
    module_color(cube, &Green);
    module_bodyColor(cube, &Green);
    module_surfaceColor(cube, &DkGrey); // Choose a surface color if needed
    module_cube(cube, 1); 

    scene = module_create();
    module_module(scene, cylinder);
    module_module(scene, cone);
    module_module(scene, sphere);
    module_module(scene, cube);

    light = lighting_create();
    //lighting_add( light, LightAmbient, &White, NULL, NULL, 0, 0 );
    lighting_add( light, LightPoint, &White, NULL, &(view.vrp), 0, 0 );
    
    ds = drawstate_create();
    point_copy(&(ds->viewer), &(view.vrp) ); // BAM you have to set the viewer location in the drawstate
    ds->shade = ShadeGouraud;

    matrix_identity(&GTM);
    module_draw(scene, &VTM, &GTM, ds, light, src);

    image_write(src, "test10b.ppm");

    module_delete(cylinder);
    module_delete(cone);
    module_delete(sphere);
    module_delete(cube);
    module_delete(scene);
    image_free(src);

    return 0;
}
