#include <math.h>
#include "matrix.h"

// 2D matrix
// print out the matrix in a nice 4x4 arrangement with a blank line below
void matrix_print(Matrix *m, FILE *fp){
    if (m == NULL || fp == NULL) {
        return;
    }   
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fprintf(fp, "%8.2f ", m->m[i][j]);
        }
        fprintf(fp, "\n");
    }
    
    fprintf(fp, "\n"); // Print a blank line below the matrix
}

// set the matrix to all zeros
void matrix_clear(Matrix *m){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m->m[i][j] = 0;
        }
    }
}

// set the matrix to the identity matrix
void matrix_identity(Matrix *m){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }

    matrix_clear(m);

    for(int i = 0; i < 4; i++){
        m->m[i][i] = 1.0;
    }
}

// return the element of the matrix at row r, column c
double matrix_get(Matrix *m, int r, int c){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return 0;
    }

    return m->m[r][c];
}

// copy the src matrix into the dest matrix
void matrix_set(Matrix *m, int r, int c, double v){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }

    m->m[r][c] = v;
}

// copy the src matrix into the dest matrix
void matrix_copy(Matrix *dest, Matrix *src){
    if (dest == NULL || src == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dest->m[i][j] = src->m[i][j];
        }
    }
}

// transpose the matrix m in place
void matrix_transpose(Matrix *m){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }

    for(int i = 0; i < 4; i++){
        for (int j = i + 1; j < 4; j++){
            double temp = m->m[i][j];
            m->m[i][j] = m->m[j][i];
            m->m[j][i] = temp;
        }
    }
}

// multiply left and right and put the result in m
// make sure the function is written so that the result matrix can also be the left or right matrix
void matrix_multiply(Matrix *left, Matrix *right, Matrix *m){
    if (left == NULL || right == NULL || m == NULL) {
        fprintf(stderr, "Invalid matrices.\n");
        return;
    }

    Matrix result; 

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] = 0.0;
            for (int k = 0; k < 4; k++) {
                result.m[i][j] += left->m[i][k] * right->m[k][j];
            }
        }
    }

    // Copy the result to the output matrix m
    matrix_copy(m, &result);
}

// transform the point p by the matrix m and put the result in q
// p and q need to be different variables
void matrix_xformPoint(Matrix *m, Point *p, Point *q){
    if (m == NULL || p == NULL || q == NULL) {
        fprintf(stderr, "Invalid matrix or points.\n");
        return;
    }

    Point temp; 

    for (int i = 0; i < 4; i++) {
        temp.val[i] = 0.0;
        for (int j = 0; j < 4; j++) {
            temp.val[i] += m->m[i][j] * p->val[j];
        }
    }
    // Copy the result to the output point q
    point_copy(q, &temp);
}

// transform the vector p by the matrix m and put the result in q.
// for this function, p and q need to be different variables
void matrix_xformVector(Matrix *m, Vector *p, Vector *q){
    if (m == NULL || p == NULL || q == NULL) {
        fprintf(stderr, "Invalid matrix or points.\n");
        return;
    }

    Vector temp; // Temporary vector to store the result

    for (int i = 0; i < 4; i++) {
        temp.val[i] = 0.0;
        for (int j = 0; j < 4; j++) {
            temp.val[i] += m->m[i][j] * p->val[j];
        }
    }

    // Copy the result to the output vector q
    vector_copy(q, &temp);
}

// transform the points and surface normals(if they exist)in the polygon p by the matrix m
void matrix_xformPolygon(Matrix *m, Polygon *p){
    if (m == NULL || p == NULL) {
        fprintf(stderr, "Invalid matrix or polygon.\n");
        return;
    }

    for (int i = 0; i < p->nVertex; i++) {
        // Transform the vertex
        Point transformedVertex;
        matrix_xformPoint(m, &p->vertex[i], &transformedVertex);
        vector_copy(&p->vertex[i], &transformedVertex);

        // Transform the normal if it exists
        if (p->normal != NULL) {
            Vector transformedNormal;
            matrix_xformVector(m, &p->normal[i], &transformedNormal);
            vector_copy(&p->normal[i], &transformedNormal);
        }
    }
}

// transform the points in the polyline p by the matrix m
void matrix_xformPolyline(Matrix *m, Polyline *p){
    if(m == NULL || p == NULL){
        fprintf(stderr, "Invalid matrix or polyline.\n");
        return;
    }

    for(int i = 0; i < p->numVertex; i++){
        Point transformedVertex;
        matrix_xformPoint(m, &p->vertex[i], &transformedVertex);
        vector_copy(&p->vertex[i], &transformedVertex);
    }
}

// transform the points in line by the matrix m
void matrix_xformLine(Matrix *m, Line *line){
    if(m == NULL || line == NULL){
        fprintf(stderr, "Invalid matrix or line.\n");
        return;
    }

    Point a;
    Point b;

    matrix_xformPoint(m, &line->a, &a);
    matrix_xformPoint(m, &line->b, &b);

    vector_copy(&line->a, &a);
    vector_copy(&line->b, &b);
}

// premultiply the matrix by a scale matrix parameterized by sx and sy
void matrix_scale2D(Matrix *m, double sx, double sy){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }

    Matrix scale;
    matrix_identity(&scale);
    scale.m[0][0] = sx;
    scale.m[1][1] = sy;

    Matrix result;
    matrix_multiply(&scale, m, &result);

    matrix_copy(m, &result);
}

// premultiply the matrix by a Z-axis rotation matrix parameterized by cos(θ) and sin(θ)
// θ is the angle of the rotation about the Z-axis
void matrix_rotateZ(Matrix *m, double cth, double sth){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }
    
    Matrix rotate;
    matrix_identity(&rotate);
    rotate.m[0][0] = cth;
    rotate.m[0][1] = -sth;
    rotate.m[1][0] = sth;
    rotate.m[1][1] = cth;
    
    Matrix result;
    matrix_multiply(&rotate, m, &result);

    matrix_copy(m, &result);
}

// premultiply the matrix by a 2D translation matrix parameterized by tx and ty
void matrix_translate2D(Matrix *m, double tx, double ty){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }

    Matrix translation;
    matrix_identity(&translation);
    translation.m[0][3] = tx;
    translation.m[1][3] = ty;

    Matrix result;
    matrix_multiply(&translation, m, &result);

    matrix_copy(m, &result);
}

// premultiply the matrix by a 2D shear matrix parameterized by shx and shy
void matrix_shear2D(Matrix *m, double shx, double shy){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }

    Matrix shear;
    matrix_identity(&shear);
    shear.m[0][1] = shx;
    shear.m[1][0] = shy;

    Matrix result;
    matrix_multiply(&shear, m, &result);

    matrix_copy(m, &result);
}

// define a function to generate a view transformation matrix(vtm)
// set the vtm to be the view transformation defined by the 2Dview structure
void matrix_setView2D(Matrix *vtm, View2D *view){
    if(vtm == NULL || view == NULL){
        fprintf(stderr, "Invalid matrix or view.\n");
        return;
    }
    matrix_identity(vtm);

    //translate the view center to the origin
    Matrix translateToOrigin;
    matrix_identity(&translateToOrigin);
    matrix_translate2D(&translateToOrigin, -view->vrp.val[0], -view->vrp.val[1]);
    matrix_multiply(&translateToOrigin, vtm, vtm);

    //orienting the view window
    Matrix rotate;
    matrix_identity(&rotate);
    vector_normalize(&(view->x));
    matrix_rotateZ(&rotate, view->x.val[0], -view->x.val[1]);
    matrix_multiply(&rotate, vtm, vtm);

    //scaling the view window
    Matrix scale;
    matrix_identity(&scale);
    double dv = view->du * view->screeny / view->screenx;
    double scaleX = view->screenx / view->du;
    double scaleY = view->screeny / dv;
    matrix_scale2D(&scale, scaleX, -scaleY);
    matrix_multiply(&scale, vtm, vtm);

    //Translation to the center of the image
    Matrix translateToCenter;
    matrix_identity(&translateToCenter);
    matrix_translate2D(&translateToCenter, view->screenx / 2.0, view->screeny / 2.0);
    matrix_multiply(&translateToCenter, vtm, vtm);
}

// 3D matrix
// premultiply the matrix by a translation matrix parameterized by tx, ty and tz
void matrix_translate(Matrix *m, double tx, double ty, double tz){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }

    Matrix translation;
    matrix_identity(&translation);
    translation.m[0][3] = tx;
    translation.m[1][3] = ty;
    translation.m[2][3] = tz;

    Matrix result;
    matrix_multiply(&translation, m, &result);

    matrix_copy(m, &result);
}

// premultiply the matrix by a scale matrix parameterized by sx, sy, sz
void matrix_scale(Matrix *m, double sx, double sy, double sz){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }

    Matrix scale;
    matrix_identity(&scale);
    scale.m[0][0] = sx;
    scale.m[1][1] = sy;
    scale.m[2][2] = sz;

    Matrix result;
    matrix_multiply(&scale, m, &result);

    matrix_copy(m, &result);
}

// premultiply the matrix by a X-axis rotation matrix parameterized by cos(θ) and sin(θ)
// θ is the angle of rotation about the X-axis
void matrix_rotateX(Matrix *m, double cth, double sth){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }
    
    Matrix rotate;
    matrix_identity(&rotate);
    rotate.m[1][1] = cth;
    rotate.m[1][2] = -sth;
    rotate.m[2][1] = sth;
    rotate.m[2][2] = cth;

    Matrix result;
    matrix_multiply(&rotate, m, &result);

    matrix_copy(m, &result);
}

// premultiply the matrix by a Y-axis rotation matrix parameterized by cos(θ) and sin(θ)
// θ is the angle of rotation about the Y-axis
void matrix_rotateY(Matrix *m, double cth, double sth){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }
    
    Matrix rotate;
    matrix_identity(&rotate);
    rotate.m[0][0] = cth;
    rotate.m[0][2] = sth;
    rotate.m[2][0] = -sth;
    rotate.m[2][2] = cth;

    Matrix result;
    matrix_multiply(&rotate, m, &result);

    matrix_copy(m, &result);
}

// premultiply the matrix by an XYZ_axis rotation matrix parameterized by the vectors u, v and w
// the vectors represent an orthonormal 3D basis
void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }

    Matrix rotation = {
        {
            {u->val[0], u->val[1], u->val[2], 0},
            {v->val[0], v->val[1], v->val[2], 0},
            {w->val[0], w->val[1], w->val[2], 0},
            {0, 0, 0, 1}
        }
    };
    
    Matrix result;
    matrix_multiply(&rotation, m, &result);

    matrix_copy(m, &result);
}

// premultiply the matrix by a shear Z matrix parameterized by shx and shy
void matrix_shearZ(Matrix *m, double shx, double shy){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }

    Matrix shear;
    matrix_identity(&shear);
    shear.m[0][2] = shx;
    shear.m[1][2] = shy;

    Matrix result = {0};
    matrix_multiply(&shear, m, &result);

    matrix_copy(m, &result);
}

// premultiply the matrix by a perspective matrix parameterized by d
void matrix_perspective(Matrix *m, double d){
    if (m == NULL) {
        fprintf(stderr, "Invalid matrix.\n");
        return;
    }

    Matrix perspective;
    matrix_identity(&perspective);
    perspective.m[3][2] = 1 / d;

    Matrix result = {0};
    matrix_multiply(&perspective, m, &result);

    matrix_copy(m, &result);
}

// 3D perspective pipeline
// when the function returns, the vtm should contain the complete view matrix
// initialize vtm to the identity
// do not modify any of the values in the perspective view inside the function(no side-effects)
void matrix_setView3D(Matrix *vtm, View3D *view){
    if(vtm == NULL || view == NULL){
        fprintf(stderr, "Invalid matrix or view.\n");
        return;
    }
    matrix_identity(vtm);

    // calculate u vector for the view reference coordinate system
    // Recalculate the VUP so that the view reference coordinates are orthogonal
    Vector u, v, w;
    vector_copy(&w, &view->vpn);
    vector_normalize(&w); // normalized vpn
    vector_cross(&view->vup, &w, &u); // calculate u vector
    vector_normalize(&u); 
    vector_cross(&w, &u, &v); // Recalculate the VUP

    // translate the VRP to the origin
    Matrix translateVRP;
    matrix_identity(&translateVRP);
    matrix_translate(&translateVRP, -view->vrp.val[0], -view->vrp.val[1], -view->vrp.val[2]);
    matrix_multiply(&translateVRP, vtm, vtm);
    
    // align the VPN with the nrgative z-axis and VUP with the y-axis
    Matrix align;
    matrix_identity(&align);
    align.m[0][0] = u.val[0]; align.m[0][1] = u.val[1]; align.m[0][2] = u.val[2];
    align.m[1][0] = v.val[0]; align.m[1][1] = v.val[1]; align.m[1][2] = v.val[2];
    align.m[2][0] = w.val[0]; align.m[2][1] = w.val[1]; align.m[2][2] = w.val[2];
    matrix_multiply(&align, vtm, vtm);

    // translate COP to the origin
    Matrix translateCOP;
    matrix_identity(&translateCOP);
    matrix_translate(&translateCOP, 0, 0, view->d);
    matrix_multiply(&translateCOP, vtm, vtm);

    // scale to canonical view volume
    double B = view->b + view->d;
    Matrix scaleCVV;
    matrix_identity(&scaleCVV);
    matrix_scale(&scaleCVV, 2 * view->d / (B * view->du), 2 * view->d / (B * view->dv), 1 / B);
    matrix_multiply(&scaleCVV, vtm, vtm);

    // project the scene onto the view plane, located a distance d along the VPN
    double d = view->d / B;
    matrix_perspective(vtm, d);

    // scale to the image size
    Matrix scaleToImage;
    matrix_identity(&scaleToImage);
    matrix_scale(&scaleToImage, -view->screenx / (2 * d), -view->screeny / (2 * d), 1.0);
    matrix_multiply(&scaleToImage, vtm, vtm);

    // translate to the screen coordinates
    Matrix translateToScreen;
    matrix_identity(&translateToScreen);
    matrix_translate(&translateToScreen, view->screenx / 2.0, view->screeny / 2.0, 0);
    matrix_multiply(&translateToScreen, vtm, vtm);
}

// 1 is zero matrix, 0 is faulse
int matrix_is_zero(Matrix *m){
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (m->m[i][j] != 0.0) {
                return 0;
            }
        }
    }
    return 1;
}