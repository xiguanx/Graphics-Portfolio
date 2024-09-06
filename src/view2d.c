#include "view2d.h"

void view2D_set(View2D *view, Point *vrp, double du, Vector *x, int screenx, int screeny){
    if(view == NULL || vrp == NULL || x == NULL){
        fprintf(stderr, "Invalid view2D, vrp or vector.\n");
        return;
    }

    point_copy(&(view->vrp), vrp);
    view->du = du;
    vector_copy(&(view->x), x);
    view->screenx = screenx;
    view->screeny = screeny;
}