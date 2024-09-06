#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "module.h"

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

  src = image_create(rows, cols);
  scene = module_create();
  matrix_identity(&GTM);
  matrix_identity(&VTM);

  // set the View
  point_set3D(&(view.vrp), 10.05, 10.75, -14.9);
  vector_set(&(view.vpn), -10.05, -10.75, 14.9);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 2.0;
  view.du = 1.6;
  view.dv =0.9;
  view.f = 0.0;
  view.b = 30;
  view.screenx = cols;
  view.screeny = rows;
  matrix_setView3D(&VTM, &view);

  // print out VTM
  printf("Final VTM: \n");
  matrix_print(&VTM, stdout);

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
  module_translate(prism, 0.01, 2.26, -2.26);
  module_color(prism, &White);
  module_bodyColor(prism, &White);
  module_surfaceColor(prism, &DkGrey);
  module_prism(prism);
  module_module(scene, prism);

  // light source from the viewpoint
  light = lighting_create();
  lighting_add(light, LightPoint, &White, NULL, &(view.vrp), 0, 0);

  ds = drawstate_create();
  point_copy(&(ds->viewer), &(view.vrp));
  ds->shade = ShadeGouraud;

  matrix_identity(&GTM);
  module_draw(scene, &VTM, &GTM, ds, light, src);

  image_write(src, "test10a.ppm");
  module_delete(cuboid1);
  module_delete(cuboid2);
  module_delete(prism);
  module_delete(scene);
  image_free(src);

  return 0;
}
