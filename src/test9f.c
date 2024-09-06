#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "plyRead.h"

int main(int argc, char *argv[]) {
    Image *src;
    Matrix VTM;
    Matrix GTM;
    Module *cube;
    int rows = 500;
    int cols = 500;
    int nPolygons;
    Polygon *plist;
    Color *clist;
    Color SurfaceColor;
    Lighting *light;
    DrawState *ds;
    View3D view;
    char buffer[256];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <ply file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Initialize the image
    src = image_create(rows, cols);

    // Initialize matrices
    matrix_identity(&GTM);
    matrix_identity(&VTM);

    // Set the View parameters
    point_set3D(&(view.vrp), 0.0, 0.0, -5.0);
    vector_set(&(view.vpn), 0.0, 0.0, 1.0);
    vector_set(&(view.vup), 0.0, 1.0, 0.0);
    view.d = 2.0;
    view.du = 1.0;
    view.dv = 1.0;
    view.f = 0.0;
    view.b = 100;
    view.screenx = cols;
    view.screeny = rows;
    matrix_setView3D(&VTM, &view);

    // Read the PLY file
    if (readPLY(argv[1], &nPolygons, &plist, &clist, 1) != 0) {
        fprintf(stderr, "Error reading PLY file\n");
        return EXIT_FAILURE;
    }

    // Create a module for the cube
    cube = module_create();
    color_set(&SurfaceColor, 0.5, 0.5, 0.5);
    module_surfaceColor(cube, &SurfaceColor);
    for (int i = 0; i < nPolygons; i++) {
        module_bodyColor(cube, &clist[i]);
        module_polygon(cube, &(plist[i]));
    }

    // Setup lighting and draw state
    light = lighting_create();
    ds = drawstate_create();
    point_copy(&(ds->viewer), &(view.vrp));
    ds->shade = ShadeGouraud;

    // Render the cube
    module_draw(cube, &VTM, &GTM, ds, light, src);

    // Write the image to a file
    sprintf(buffer, "cube_render.ppm");
    image_write(src, buffer);

    // Clean up
    image_free(src);
    module_delete(cube);
    free(plist);
    free(clist);
    lighting_delete(light);

    return EXIT_SUCCESS;
}