#include <stdlib.h>
#include <math.h>
#include "module.h"

// 2D module functions
// allocate and return an initialized but empty element
Element *element_create(){
    Element *e = (Element*)malloc(sizeof(Element));
    if(e == NULL){
        fprintf(stderr, "Unable to allocate memory for element.\n");
        return NULL;
    }

    e->type = ObjNone;
    e->obj = NULL;
    e->next = NULL;

    return e;
}

// allocate an Element and store a duplicate of the data pointed to by obj in the Element
// Modules do not get duplicated.
// the function needs to handle each type of object separately in a case statement
Element *element_init(ObjectType type, void *obj){
    if(obj == NULL && type != ObjIdentity){
        fprintf(stderr, "Invalid obj.\n");
        return NULL;
    }
    Element *e = element_create();

    e->type = type;
    switch(type){
        case ObjNone:
            e->obj = NULL;
            break;
        case ObjLine:
            e->obj = (Line*)malloc(sizeof(Line));
            if(e->obj == NULL){
                fprintf(stderr, "Unable to allocate memory for obj.\n");
                return NULL;
            }
            line_copy(e->obj, obj);
            break;
        case ObjPoint:
            e->obj = (Point*)malloc(sizeof(Point));
            if(e->obj == NULL){
                fprintf(stderr, "Unable to allocate memory for obj.\n");
                return NULL;
            }
            point_copy(e->obj, obj);
            break;
        case ObjPolyline:
            e->obj = polyline_create();
            polyline_copy(e->obj, obj);
            break;
        case ObjPolygon:
            e->obj = polygon_create();
            polygon_copy(e->obj, obj);
            break;
        case ObjIdentity:
            e->obj = NULL;
            break;
        case ObjMatrix:
            e->obj = (Matrix*)malloc(sizeof(Matrix));
            if(e->obj == NULL){
                fprintf(stderr, "Unable to allocate memory for obj.\n");
                return NULL;
            }
            matrix_copy(e->obj, obj);
            break;
        case ObjColor:
            e->obj = (Color*)malloc(sizeof(Color));
            if(e->obj == NULL){
                fprintf(stderr, "Unable to allocate memory for obj.\n");
                return NULL;
            }
            color_copy(e->obj, obj);
            break;
        case ObjBodyColor:
            e->obj = (Color*)malloc(sizeof(Color));
            if(e->obj == NULL){
                fprintf(stderr, "Unable to allocate memory for obj.\n");
                return NULL;
            }
            color_copy(e->obj, obj);
            break;
        case ObjSurfaceColor:
            e->obj = (Color*)malloc(sizeof(Color));
            if(e->obj == NULL){
                fprintf(stderr, "Unable to allocate memory for obj.\n");
                return NULL;
            }
            color_copy(e->obj, obj);
            break;
        case ObjSurfaceCoeff:
            e->obj = (float*)malloc(sizeof(float));
            e->obj = (float*)obj;
            break;
        case ObjLight:
            e->obj = (Light*)malloc(sizeof(Light));
             if(e->obj == NULL){
                fprintf(stderr, "Unable to allocate memory for obj.\n");
                return NULL;
            }
            light_copy(e->obj, obj);
            break;
        case ObjModule:
            e->obj = obj;
            break;
    }

    return e;
}

// free the element and the object it contains as appropriate
void element_delete(Element *e){
    if(e == NULL){
        fprintf(stderr, "Invalid element.\n");
        return;
    }
    switch(e->type){
        case ObjPolyline:
            polyline_free(e->obj);
            break;
        case ObjPolygon:
            polygon_free(e->obj);
            break;
        case ObjLine:
        case ObjPoint:
        case ObjMatrix:
        case ObjColor:
        case ObjBodyColor:
        case ObjSurfaceColor:
        case ObjSurfaceCoeff:
        case ObjModule:
        case ObjLight:
            free(e->obj);
            break;
        case ObjNone:
        case ObjIdentity:
            break;
    }
    free(e);
}

// allocate an empty module
Module *module_create(){
    Module *m = (Module*)malloc(sizeof(Module));
    if(m == NULL){
        fprintf(stderr, "Unable to allocate memory for module.\n");
        return NULL;
    }

    m->head = NULL;
    m->tail = NULL;

    return m;
}

// clear the module's list of elements, freeing memory as appropriate
void module_clear(Module *md){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Element *current = md->head;
    while(current != NULL){
        element_delete(current);
        current = current->next;
    }

    md->head = NULL;
    md->tail = NULL;
}

// free all of the memory associated with a module, including the memory pointed to by md
void module_delete(Module *md){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }
    
    Element *current = md->head;
    while(current != NULL){
        element_delete(current);
        current = current->next;
    }

    free(md);
}

// generic insert of an element into the module at the tail of the list
void module_insert(Module *md, Element *e){
    if(md == NULL || e == NULL){
        fprintf(stderr, "Invalid module or element.\n");
        return;
    }

    if(md->tail == NULL){
        md->head = e;
        md->tail = e;
    }else{
        md->tail->next = e;
        md->tail = e;
    }
    md->tail->next = NULL;
}

// add a pointer to the module sub to the tail of the module's list
void module_module(Module *md, Module *sub){
    if(md == NULL || sub == NULL){
        fprintf(stderr, "Invalid modules.\n");
        return;
    }

    Element *e = element_init(ObjModule, sub);
    module_insert(md, e);
}

// add p to the tail of the module's list
void module_point(Module *md, Point *p){
    if(md == NULL || p == NULL){
        fprintf(stderr, "Invalid module or point.\n");
        return;
    }

    Element *e = element_init(ObjPoint, p);
    module_insert(md, e);
}

// add p to the tail of the module's list
void module_line(Module *md, Line *p){
    if(md == NULL || p == NULL){
        fprintf(stderr, "Invalid module or line.\n");
        return;
    }

    Element *e = element_init(ObjLine, p);
    module_insert(md, e);
}

// add p to the tail of the module's list
void module_polyline(Module *md, Polyline *p){
    if(md == NULL || p == NULL){
        fprintf(stderr, "Invalid module or polyline.\n");
        return;
    }

    Element *e = element_init(ObjPolyline, p);
    module_insert(md, e);
}

// add p to the tail of the module's list
void module_polygon(Module *md, Polygon *p){
    if(md == NULL || p == NULL){
        fprintf(stderr, "Invalid module or polygon.\n");
        return;
    }

    Element *e = element_init(ObjPolygon, p);
    module_insert(md, e);
}

// object that sets the current transform to the identity, placed at the tail of the module's list
void module_identity(Module *md){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Element *e = element_init(ObjIdentity, NULL);
    module_insert(md, e);
}

// matrix operand to add a translation matrix to the tail of the module's list
void module_translate2D(Module *md, double tx, double ty){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Matrix m;
    matrix_identity(&m);
    matrix_translate2D(&m, tx, ty);
    Element *e = element_init(ObjMatrix, &m);
    module_insert(md, e);
}

// matrix operand to add a scale matrix to the tail of the module's list
void module_scale2D(Module *md, double sx, double sy){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Matrix m;
    matrix_identity(&m);
    matrix_scale2D(&m, sx, sy);
    Element *e = element_init(ObjMatrix, &m);
    module_insert(md, e);
}

// matrix operand to add a rotation about Z axis to the tail of the module's list
void module_rotateZ(Module *md, double cth, double sth){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Matrix m;
    matrix_identity(&m);
    matrix_rotateZ(&m, cth, sth);
    Element *e = element_init(ObjMatrix, &m);
    module_insert(md, e);
}

// matrix operand to add a 2D shear matrix to the tail of the module's list
void module_shear2D(Module *md, double shx, double shy){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Matrix m;
    matrix_identity(&m);
    matrix_shear2D(&m, shx, shy);
    Element *e = element_init(ObjMatrix, &m);
    module_insert(md, e);
}

// draw the module into the image using the given view transformation matrix VTM
// Lighting and DrawState by traversing the list of Elements
// Lighting can be an empty structure
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Matrix LTM;
    matrix_identity(&LTM);
    
    Element *current = md->head;
    while(current != NULL){
        
        switch(current->type){
            case ObjNone:       
                break;        
            case ObjColor:
                color_copy(&(ds->color), current->obj);
                break;
            case ObjBodyColor:
                color_copy(&(ds->bodyColor), current->obj);
                break;
            case ObjSurfaceColor:
                color_copy(&(ds->surfaceColor), current->obj);
                break;
            case ObjSurfaceCoeff:
                ds->surfaceCoeff = *(float*)(current->obj);
                break;
            case ObjPoint: {
                Point X, q;
                point_copy(&X, current->obj);
                matrix_xformPoint(&LTM, &X, &q);
                matrix_xformPoint(GTM, &q, &X);
                matrix_xformPoint(VTM, &X, &q);
                point_normalize(&q);
                point_draw(&q, src, ds->color);
                break;
            }                
            case ObjLine: {
                Line L;
                line_copy(&L, current->obj);
                matrix_xformLine(&LTM, &L);
                matrix_xformLine(GTM, &L);
                matrix_xformLine(VTM, &L);
                line_normalize(&L);
                printf("drawing line (%.2f %.2f) to (%.2f %.2f)\n", L.a.val[0], L.a.val[1], 
                        L.b.val[0], L.b.val[1] );
                line_draw(&L, src, ds->color);
                break;
            }
            case ObjPolyline: {
                Polyline polyline;
                polyline_init(&polyline);
                polyline_copy(&polyline, current->obj);
                matrix_xformPolyline(&LTM, &polyline);
                matrix_xformPolyline(GTM, &polyline);
                matrix_xformPolyline(VTM, &polyline);
                polyline_normalize(&polyline);
                polyline_draw(&polyline, src, ds->color);
                break;
            }
            case ObjPolygon:{
                Polygon plg;
                polygon_init(&plg);
                polygon_copy(&plg, current->obj);
                matrix_xformPolygon(&LTM, &plg);
                matrix_xformPolygon(GTM, &plg);
                if(ds->shade == ShadeGouraud){
                    polygon_shade(&plg, ds, lighting);
                }
                matrix_xformPolygon(VTM, &plg);
                polygon_normalize(&plg);
                switch(ds->shade){
                    case ShadeFrame:
                        polygon_draw(&plg, src, ds->color);
                        break;
                    case ShadeConstant:
                        polygon_drawFill(&plg, src, ds->color);
                        break;
                    case ShadeFlat:
                    case ShadeDepth:
                    case ShadeGouraud:
                    case ShadePhong:
                        polygon_drawShade(&plg, src, ds, lighting);
                        break;
                    default:
                        break;
                }
                break;
            }
            case ObjMatrix: {
                if(matrix_is_zero(current->obj) == 0){
                    matrix_multiply(current->obj, &LTM, &LTM);
                }             
                break;
            }
            case ObjIdentity:
                matrix_identity(&LTM);
                break;
            case ObjLight:
                if(lighting != NULL && lighting->nLights < 64) {
                    lighting->light[lighting->nLights] = *(Light*)(current->obj);
                    lighting->nLights++;
                }
                break;
            case ObjModule:{
                Matrix TM;
                DrawState tempDS;
                Lighting tempLighting;
                matrix_identity(&TM);
                matrix_multiply(GTM, &LTM, &TM);
                drawstate_copy(&tempDS, ds);
                lighting_copy(&tempLighting, lighting);
                module_draw(current->obj, VTM, &TM, &tempDS, lighting, src);
                break;
            }
        }
        current = current->next;
    }
}

// 3D module functions
// matrix operand to add a 3D translation to the module
void module_translate(Module *md, double tx, double ty, double tz){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Matrix m;
    matrix_identity(&m);
    matrix_translate(&m, tx, ty, tz);
    Element *e = element_init(ObjMatrix, &m);
    module_insert(md, e);
}

// matrix operand to add a 3D scale to the module
void module_scale(Module *md, double sx, double sy, double sz){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Matrix m;
    matrix_identity(&m);
    matrix_scale(&m, sx, sy, sz);
    Element *e = element_init(ObjMatrix, &m);
    module_insert(md, e);
}

// matrix operand to add a rotation about X axis to the module
void module_rotateX(Module *md, double cth, double sth){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Matrix m;
    matrix_identity(&m);
    matrix_rotateX(&m, cth, sth);
    Element *e = element_init(ObjMatrix, &m);
    module_insert(md, e);
}

// matrix operand to add a rotation about Y axis to the module
void module_rotateY(Module *md, double cth, double sth){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Matrix m;
    matrix_identity(&m);
    matrix_rotateY(&m, cth, sth);
    Element *e = element_init(ObjMatrix, &m);
    module_insert(md, e);
}

// matrix operand to add a rotation that orients to the orthonormal axes u, v, w
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Matrix m;
    matrix_identity(&m);
    matrix_rotateXYZ(&m, u, v, w);
    Element *e = element_init(ObjMatrix, &m);
    module_insert(md, e);
}

// shading/color module functions
// add the foreground color value to the tail of the module's list
void module_color(Module *md, Color *c){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Element *e = element_init(ObjColor, c);
    module_insert(md, e);
}

// add the body color value to the tail of the module's list
void module_bodyColor(Module *md, Color *c){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Element *e = element_init(ObjBodyColor, c);
    module_insert(md, e);
}

// add the surface color value to the tail of the module's list
void module_surfaceColor(Module *md, Color *c){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Element *e = element_init(ObjSurfaceColor, c);
    module_insert(md, e);
}

// add the specular coefficient to the tail of the module's list
void module_surfaceCoeff(Module *md, float coeff){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Element *e = element_init(ObjSurfaceCoeff, &coeff);
    module_insert(md, e);
}

// add the Light struct to the module. Copy the information in the Light struct
void module_addLight(Module *m, Light *light){
     if(m == NULL || light == NULL){
        fprintf(stderr, "Invalid module or light.\n");
        return;
    }

    Element *e = element_init(ObjLight, light);
    module_insert(m, e);
}

/*
// traverse the module and all sub-modules, keep track of the LTM and GTM and apply all ObjMatrix and ObjIdentity elements.Recursively traverse all ObjModule elements.
// when the traversal finds an ObjLight element, copy and add it to the Lighting structure and transform the position and direction fields of the Light by the LTM and GTM.
void module_parseLighting(Module *m, Matrix *GTM, Lighting *lighting){

}
*/

// use the de Casteljau algorithm to subdivide the Bezier curve divisions times
// then add the lines connecting the control points to the module
// For example, if divisions is 1, the four original Bezier curve control points will be used to generate eight control points and two new Bezier curves.
// Then the algorithm will add six lines to the module, three for each of the smaller Bezier curves.
void module_bezierCurve(Module *m, BezierCurve *b, int divisions){
    if (m == NULL || b == NULL) {
        fprintf(stderr, "Invalid module or bezier curve.\n");
        return;
    }

    if (divisions == 0) {
        // Base case: Add lines for the original BezierCurve
        for (int i = 0; i < 3; i++) {
            Line l;
            line_set(&l,b->p[i], b->p[i + 1]);
            Element *e = element_init(ObjLine, &l);
            module_insert(m, e);
        }
    } else {
        // Recursive case: Subdivide and add lines for the subdivided curves
        BezierCurve left, right;
        de_casteljau(b, &left, &right);
        module_bezierCurve(m, &left, divisions - 1);
        module_bezierCurve(m, &right, divisions - 1);
    }
}

// subdivide a BezierSurface using de Casteljau algorithm
void bezierSurface_subdivide(BezierSurface *b, BezierSurface *topLeft, BezierSurface *topRight,
                            BezierSurface *bottomLeft, BezierSurface *bottomRight) {
    Point rowMidpoints[4][3];
    Point colMidpoints[3][4];
    Point centerMidpoints[3][3];
    
        // Calculate midpoints for rows
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 3; i++) {
            for (int k = 0; k < 4; k++) {
                rowMidpoints[j][i].val[k] = (b->p[j * 4 + i].val[k] + b->p[j * 4 + i + 1].val[k]) / 2.0;
            }
        }
    }

    // Calculate midpoints for columns
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 4; i++) {
            for (int k = 0; k < 4; k++) {
                colMidpoints[j][i].val[k] = (b->p[j * 4 + i].val[k] + b->p[(j + 1) * 4 + i].val[k]) / 2.0;
            }
        }
    }

    // Calculate center midpoints
    for (int j = 0; j < 3; ++j) {
        for (int i = 0; i < 3; ++i) {
            for (int k = 0; k < 4; ++k) {
                centerMidpoints[j][i].val[k] = (rowMidpoints[j][i].val[k] + rowMidpoints[j + 1][i].val[k]) / 2.0;
            }
        }
    }

    // Set up topLeft
    point_copy(&(topLeft->p[0]), &(b->p[0]));
    point_copy(&(topLeft->p[1]), &(rowMidpoints[0][0]));
    point_copy(&(topLeft->p[2]), &(b->p[1]));
    point_copy(&(topLeft->p[3]), &(rowMidpoints[0][1]));

    point_copy(&(topLeft->p[4]), &(colMidpoints[0][0]));
    point_copy(&(topLeft->p[5]), &(centerMidpoints[0][0]));
    point_copy(&(topLeft->p[6]), &(colMidpoints[0][1]));
    point_copy(&(topLeft->p[7]), &(centerMidpoints[0][1]));

    point_copy(&(topLeft->p[8]), &(b->p[4]));
    point_copy(&(topLeft->p[9]), &(rowMidpoints[1][0])); 
    point_copy(&(topLeft->p[10]), &(b->p[5])); 
    point_copy(&(topLeft->p[11]), &(rowMidpoints[1][1]));

    point_copy(&(topLeft->p[12]), &(colMidpoints[1][0]));
    point_copy(&(topLeft->p[13]), &(centerMidpoints[1][0]));
    point_copy(&(topLeft->p[14]), &(colMidpoints[1][1]));
    point_copy(&(topLeft->p[15]), &(centerMidpoints[1][1]));

    // Set up topRight
    point_copy(&(topRight->p[0]), &(rowMidpoints[0][1]));
    point_copy(&(topRight->p[1]), &(b->p[2])); 
    point_copy(&(topRight->p[2]), &(rowMidpoints[0][2]));
    point_copy(&(topRight->p[3]), &(b->p[3]));

    point_copy(&(topRight->p[4]), &(centerMidpoints[0][1]));
    point_copy(&(topRight->p[5]), &(colMidpoints[0][2]));
    point_copy(&(topRight->p[6]), &(centerMidpoints[0][2]));
    point_copy(&(topRight->p[7]), &(colMidpoints[0][3]));

    point_copy(&(topRight->p[8]), &(rowMidpoints[1][1])); 
    point_copy(&(topRight->p[9]), &(b->p[6])); 
    point_copy(&(topRight->p[10]), &(rowMidpoints[1][2])); 
    point_copy(&(topRight->p[11]), &(b->p[7]));

    point_copy(&(topRight->p[12]), &(centerMidpoints[1][1]));
    point_copy(&(topRight->p[13]), &(colMidpoints[1][2]));
    point_copy(&(topRight->p[14]), &(centerMidpoints[1][2]));
    point_copy(&(topRight->p[15]), &(colMidpoints[1][3]));

    // Set up bottomLeft
    point_copy(&(bottomLeft->p[0]), &(colMidpoints[1][0]));
    point_copy(&(bottomLeft->p[1]), &(centerMidpoints[1][0]));
    point_copy(&(bottomLeft->p[2]), &(colMidpoints[1][1]));
    point_copy(&(bottomLeft->p[3]), &(centerMidpoints[1][1]));

    point_copy(&(bottomLeft->p[4]), &(b->p[8])); 
    point_copy(&(bottomLeft->p[5]), &(rowMidpoints[2][0])); 
    point_copy(&(bottomLeft->p[6]), &(b->p[9])); 
    point_copy(&(bottomLeft->p[7]), &(rowMidpoints[2][1])); 

    point_copy(&(bottomLeft->p[8]), &(colMidpoints[2][0]));
    point_copy(&(bottomLeft->p[9]), &(centerMidpoints[2][0]));
    point_copy(&(bottomLeft->p[10]), &(colMidpoints[2][1]));
    point_copy(&(bottomLeft->p[11]), &(centerMidpoints[2][1]));

    point_copy(&(bottomLeft->p[12]), &(b->p[12]));
    point_copy(&(bottomLeft->p[13]), &(rowMidpoints[3][0]));
    point_copy(&(bottomLeft->p[14]), &(b->p[13])); 
    point_copy(&(bottomLeft->p[15]), &(rowMidpoints[3][1]));

    // Set up bottomRight
    point_copy(&(bottomRight->p[0]), &(centerMidpoints[1][1]));
    point_copy(&(bottomRight->p[1]), &(colMidpoints[1][2]));
    point_copy(&(bottomRight->p[2]), &(centerMidpoints[1][2]));
    point_copy(&(bottomRight->p[3]), &(colMidpoints[1][3]));

    point_copy(&(bottomRight->p[4]), &(rowMidpoints[2][1])); 
    point_copy(&(bottomRight->p[5]), &(b->p[10])); 
    point_copy(&(bottomRight->p[6]), &(rowMidpoints[2][2]));
    point_copy(&(bottomRight->p[7]), &(b->p[11]));

    point_copy(&(bottomRight->p[8]), &(centerMidpoints[2][1]));
    point_copy(&(bottomRight->p[9]), &(colMidpoints[2][2])); 
    point_copy(&(bottomRight->p[10]), &(centerMidpoints[2][2]));
    point_copy(&(bottomRight->p[11]), &(colMidpoints[2][3]));

    point_copy(&(bottomRight->p[12]), &(rowMidpoints[3][1]));
    point_copy(&(bottomRight->p[13]), &(b->p[14])); 
    point_copy(&(bottomRight->p[14]), &(rowMidpoints[3][2]));
    point_copy(&(bottomRight->p[15]), &(b->p[15]));
}

// use the de Casteljau algorithm to subdivide the Bezier surface divisions times
// then add to the module either the lines connecting the control points, if solid is 0, or triangles using the four corner control points.
// For example, if divisions is 1, the 16 original Bezier curve control points will be used to generate 64 control points and four new Bezier surfaces, which is 1 level of subdivision
// then the algorithm will add lines or triangles connecting adjacent control points to the module
// Subdividing surfaces is a counting exercise
void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid){
    if (m == NULL || b == NULL) {
        fprintf(stderr, "Invalid module or bezier surface.\n");
        return;
    }

    if (divisions == 0) {
        // Base case: Add triangles or lines for the original BezierSurface
        if (solid == 0) {
        // Add lines to the Module for the BezierSurface
            for (int j = 0; j < 4; j++) {
                for (int i = 0; i < 3; i++) {
                    Line l1;
                    line_set(&l1, b->p[j * 4 + i], b->p[j * 4 + i + 1]);
                    Element *e1 = element_init(ObjLine, &l1);
                    module_insert(m, e1);

                    Line l2;
                    line_set(&l2, b->p[i * 4 + j], b->p[(i + 1) * 4 + j]);
                    Element *e2 = element_init(ObjLine, &l2);
                    module_insert(m, e2);
                }
            }
        } else {
            // Add triangles to the Module for the BezierSurface
            for (int j = 0; j < 3; ++j) {
                for (int i = 0; i < 3; ++i) {
                    Polygon *poly1 = polygon_create();
                    Point pt[3];
                    point_copy(&pt[1], &(b->p[j * 4 + i]));
                    point_copy(&pt[1], &(b->p[j * 4 + i + 1]));
                    point_copy(&pt[1], &(b->p[(j + 1) * 4 + i + 1]));

                    polygon_set(poly1, 3, pt);
                    Element *e3 = element_init(ObjPolygon, poly1);
                    module_insert(m, e3);

                    Polygon *poly2 = polygon_create();
                    point_copy(&pt[1], &(b->p[j * 4 + i]));
                    point_copy(&pt[1], &(b->p[(j + 1) * 4 + i]));
                    point_copy(&pt[1], &(b->p[(j + 1) * 4 + i + 1]));
                    polygon_set(poly2, 3, pt);
                    Element *e4 = element_init(ObjPolygon, poly2);
                    module_insert(m, e4);
                }
            }
        }
    } else {
        // Recursive case: Subdivide and add triangles or lines for the subdivided surfaces
        BezierSurface topLeft, topRight, bottomLeft, bottomRight;
        bezierSurface_subdivide(b, &topLeft, &topRight, &bottomLeft, &bottomRight);
        module_bezierSurface(m, &topLeft, divisions - 1, solid);
        module_bezierSurface(m, &topRight, divisions - 1, solid);
        module_bezierSurface(m, &bottomLeft, divisions - 1, solid);
        module_bezierSurface(m, &bottomRight, divisions - 1, solid);
    }
}

// add a unit cube, axis-aligned and centered on zero to the Module. If solid is zero, add only lines
// if solid is non-zero, use polygons. Make sure each polygon has surface normals defined for it
void module_cube(Module *md, int solid){
    if(md == NULL){
        fprintf(stderr, "Invalid module.\n");
        return;
    }

    Module *cubeLine = module_create();
    Module *cubePolygon = module_create();
    Polygon side[6];
    Line lines[12];
    Point  tv[4];
    Point  v[8];
    Vector normals[6];

    for(int n = 0; n < 6; n++){
        polygon_init(&side[n]);
	    polygon_setSided( &side[n], 1 ); // BAM might as well set them to 1-sided polygons.
    }
    // corners of a cube, centered at (0, 0, 0)
    point_set( &v[0], -0.5, -0.5, -0.5, 1 );
    point_set( &v[1],  0.5, -0.5, -0.5, 1 );
    point_set( &v[2],  0.5,  0.5, -0.5, 1 );
    point_set( &v[3], -0.5,  0.5, -0.5, 1 );
    point_set( &v[4], -0.5, -0.5,  0.5, 1 );
    point_set( &v[5],  0.5, -0.5,  0.5, 1 );
    point_set( &v[6],  0.5,  0.5,  0.5, 1 );
    point_set( &v[7], -0.5,  0.5,  0.5, 1 );

    // normals for each face
    vector_set(&normals[0], 0, 0, -1); // Front
    vector_set(&normals[1], 0, 0, 1);  // Back
    vector_set(&normals[2], 0, 1, 0);  // Top
    vector_set(&normals[3], 0, -1, 0); // Bottom
    vector_set(&normals[4], -1, 0, 0); // Left
    vector_set(&normals[5], 1, 0, 0);  // Right

    // front side
    polygon_set( &side[0], 4, &(v[0]) );
    Vector front_normals[4] = {normals[0], normals[0], normals[0], normals[0]};
    polygon_setNormals(&side[0], 4, front_normals);
    // back side
    polygon_set( &side[1], 4, &(v[4]) );
    Vector back_normals[4] = {normals[1], normals[1], normals[1], normals[1]};
    polygon_setNormals(&side[1], 4, back_normals);
    // top side
    point_copy( &tv[0], &v[2] );
    point_copy( &tv[1], &v[3] );
    point_copy( &tv[2], &v[7] );
    point_copy( &tv[3], &v[6] );
    polygon_set( &side[2], 4, tv );
    Vector top_normals[4] = {normals[2], normals[2], normals[2], normals[2]};
    polygon_setNormals(&side[2], 4, top_normals);

    // bottom side
    point_copy( &tv[0], &v[0] );
    point_copy( &tv[1], &v[1] );
    point_copy( &tv[2], &v[5] );
    point_copy( &tv[3], &v[4] );
    polygon_set( &side[3], 4, tv );
    Vector bottom_normals[4] = {normals[3], normals[3], normals[3], normals[3]};
    polygon_setNormals(&side[3], 4, bottom_normals);
    // left side
    point_copy( &tv[0], &v[0] );
    point_copy( &tv[1], &v[3] );
    point_copy( &tv[2], &v[7] );
    point_copy( &tv[3], &v[4] );
    polygon_set( &side[4], 4, tv );
    Vector left_normals[4] = {normals[4], normals[4], normals[4], normals[4]};
    polygon_setNormals(&side[4], 4, left_normals);
    // right side
    point_copy( &tv[0], &v[1] );
    point_copy( &tv[1], &v[2] );
    point_copy( &tv[2], &v[6] );
    point_copy( &tv[3], &v[5] );
    polygon_set( &side[5], 4, tv );
    Vector right_normals[4] = {normals[5], normals[5], normals[5], normals[5]};
    polygon_setNormals(&side[5], 4, right_normals);

    for(int i = 0; i < 6; i++){
        module_polygon(cubePolygon, &side[i]);
    }

    // lines set
    line_set(&lines[0], v[0], v[1]);
    line_set(&lines[1], v[0], v[3]);
    line_set(&lines[2], v[0], v[4]);
    line_set(&lines[3], v[1], v[2]);
    line_set(&lines[4], v[1], v[5]);
    line_set(&lines[5], v[2], v[3]);
    line_set(&lines[6], v[2], v[6]);
    line_set(&lines[7], v[3], v[7]);
    line_set(&lines[8], v[4], v[5]);
    line_set(&lines[9], v[4], v[7]);
    line_set(&lines[10], v[5], v[6]);
    line_set(&lines[11], v[6], v[7]);

    for(int j = 0; j < 12; j++){
        module_line(cubeLine, &lines[j]);
    }
  
    if(solid == 0){
        module_module(md, cubeLine);
    }else{
        module_module(md, cubePolygon);
    }
}

// create a cylinder module
void module_cylinder( Module *mod, int sides ) {
  Polygon p;
  Point xtop, xbot;
  Vector normal_top, normal_bot;
  double x1, x2, z1, z2;
  int i;

  polygon_init( &p );
  polygon_setSided( &p, 1 ); // BAM might as well set them to 1-sided polygons.
  
  point_set3D( &xtop, 0, 1.0, 0.0 );
  point_set3D( &xbot, 0, 0.0, 0.0 );
  vector_set(&normal_top, 0, 1, 0); // pointing upwards
  vector_set(&normal_bot, 0, -1, 0); // pointing downwards

  // make a fan for the top and bottom sides
  // and quadrilaterals for the sides
  for(i=0;i<sides;i++) {
    Point pt[4];
    Vector side_normals[4];

    x1 = cos( i * M_PI * 2.0 / sides );
    z1 = sin( i * M_PI * 2.0 / sides );
    x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
    z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

    // top
    point_copy( &pt[0], &xtop );
    point_set3D( &pt[1], x1, 1.0, z1 );
    point_set3D( &pt[2], x2, 1.0, z2 );

    polygon_set( &p, 3, pt );
    polygon_setNormals(&p, 3, (Vector[]){normal_top, normal_top, normal_top});
    module_polygon( mod, &p );

    // bottom
    point_copy( &pt[0], &xbot );
    point_set3D( &pt[1], x1, 0.0, z1 );
    point_set3D( &pt[2], x2, 0.0, z2 );

    polygon_set( &p, 3, pt );
    polygon_setNormals(&p, 3, (Vector[]){normal_bot, normal_bot, normal_bot});
    module_polygon( mod, &p );

    // side
    point_set3D( &pt[0], x1, 0.0, z1 );
    point_set3D( &pt[1], x2, 0.0, z2 );
    point_set3D( &pt[2], x2, 1.0, z2 );
    point_set3D( &pt[3], x1, 1.0, z1 );
    
    vector_set(&side_normals[0], x1, 0, z1);
    vector_set(&side_normals[1], x2, 0, z2);
    vector_set(&side_normals[2], x2, 0, z2);
    vector_set(&side_normals[3], x1, 0, z1);
    polygon_set( &p, 4, pt );
    polygon_setNormals(&p, 4, side_normals);
    module_polygon( mod, &p );
  }

  polygon_clear( &p );
}

// a unit cone module
void module_cone(Module *mod, int sides){
    Polygon p;
    Point xtop, xbot;
    Vector normal_bot;
    double x1, x2, z1, z2;

    polygon_init(&p);
    point_set3D( &xtop, 0, 1.0, 0.0 );
    point_set3D( &xbot, 0, 0.0, 0.0 );
    vector_set(&normal_bot, 0, -1, 0);

    // make fans for the bottom and side
    for(int i = 0; i < sides; i++){
        Point pt[3];
        Vector side_normals[3];
        x1 = cos(i * M_PI * 2.0 / sides);
        z1 = sin( i * M_PI * 2.0 / sides );
        x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
        z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

        // bottom
        point_copy( &pt[0], &xbot );
        point_set3D( &pt[1], x1, 0, z1 );
        point_set3D( &pt[2], x2, 0, z2 );

        polygon_set( &p, 3, pt );
        polygon_setNormals(&p, 3, (Vector[]){normal_bot, normal_bot, normal_bot});
        module_polygon( mod, &p );

        // side
        point_copy( &pt[0], &xtop );
        point_set3D( &pt[1], x1, 0, z1 );
        point_set3D( &pt[2], x2, 0, z2 );

        vector_set(&side_normals[0], x1, 0.5, z1);
        vector_set(&side_normals[1], x1, 0.5, z1);
        vector_set(&side_normals[2], x2, 0.5, z2);
        polygon_set( &p, 3, pt );
        polygon_setNormals(&p, 3, side_normals);
        module_polygon( mod, &p );
    }

    polygon_clear(&p);
}

// a unit sphere module
void module_sphere(Module *mod, int slices, int stacks) {
    Polygon p;
    Point pt[4];
    Vector n[4];
    double phi1, phi2, theta1, theta2;
    double x1, y1, z1, x2, y2, z2;
    int i, j;

    polygon_init(&p); 

    for(i = 0; i < stacks; i++) {
        phi1 = M_PI * (-0.5 + (double)(i) / stacks);
        phi2 = M_PI * (-0.5 + (double)(i + 1) / stacks);

        for(j = 0; j < slices; j++) {
            theta1 = 2 * M_PI * (double)(j) / slices;
            theta2 = 2 * M_PI * (double)(j + 1) / slices;

            //set up 4 points of polygon
            x1 = cos(phi1) * cos(theta1);
            y1 = sin(phi1);
            z1 = cos(phi1) * sin(theta1);
            point_set3D(&pt[0], x1, y1, z1);
            vector_set(&n[0], x1, y1, z1);

            x2 = cos(phi1) * cos(theta2);
            y2 = sin(phi1);
            z2 = cos(phi1) * sin(theta2);
            point_set3D(&pt[1], x2, y2, z2);
            vector_set(&n[1], x2, y2, z2);

            x1 = cos(phi2) * cos(theta2);
            y1 = sin(phi2);
            z1 = cos(phi2) * sin(theta2);
            point_set3D(&pt[2], x1, y1, z1);
            vector_set(&n[2], x1, y1, z1);

            x2 = cos(phi2) * cos(theta1);
            y2 = sin(phi2);
            z2 = cos(phi2) * sin(theta1);
            point_set3D(&pt[3], x2, y2, z2);
            vector_set(&n[3], x2, y2, z2);

            polygon_set(&p, 3, pt); 
            polygon_setNormals(&p, 3, n);
            module_polygon(mod, &p);

            point_copy(&pt[1], &pt[3]);
            vector_copy(&n[1], &n[3]);
            polygon_set(&p, 3, pt);
            polygon_setNormals(&p, 3, n);
            module_polygon(mod, &p);
        }
    }

    polygon_clear(&p); 
}

void module_prism( Module *mod ) {
    Module *prism = module_create();
    Polygon side[6];
    Point  tv[4];
    Point  v[8];
    Vector normals[6];

    for(int n = 0; n < 6; n++){
        polygon_init(&side[n]);
	    polygon_setSided( &side[n], 1 ); // BAM might as well set them to 1-sided polygons.
    }
    // vertices, centered at (0, 0, 0)
    point_set( &v[0], -0.5, -0.5, -0.5, 1 );
    point_set( &v[1],  0.5, -0.5, -0.5, 1 );
    point_set( &v[2],  1.0,  1.0, -0.5, 1 );
    point_set( &v[3], -0.5,  0.5, -0.5, 1 );
    point_set( &v[4], -0.5, -0.5,  0.5, 1 );
    point_set( &v[5],  0.5, -0.5,  0.5, 1 );
    point_set( &v[6],  1.0,  1.0,  0.5, 1 );
    point_set( &v[7], -0.5,  0.5,  0.5, 1 );

    // normals for each face
    vector_set(&normals[0], 0, 0, -1); // Front
    vector_set(&normals[1], 0, 0, 1);  // Back
    vector_set(&normals[2], 0, 1, 0);  // Top
    vector_set(&normals[3], 0, -1, 0); // Bottom
    vector_set(&normals[4], -1, 0, 0); // Left
    vector_set(&normals[5], 0.5, -1.0, 0); // Right

    // front side
    polygon_set( &side[0], 4, &(v[0]) );
    Vector front_normals[4] = {normals[0], normals[0], normals[0], normals[0]};
    polygon_setNormals(&side[0], 4, front_normals);
    // back side
    polygon_set( &side[1], 4, &(v[4]) );
    Vector back_normals[4] = {normals[1], normals[1], normals[1], normals[1]};
    polygon_setNormals(&side[1], 4, back_normals);
    // top side
    point_copy( &tv[0], &v[2] );
    point_copy( &tv[1], &v[3] );
    point_copy( &tv[2], &v[7] );
    point_copy( &tv[3], &v[6] );
    polygon_set( &side[2], 4, tv );
    Vector top_normals[4] = {normals[2], normals[2], normals[2], normals[2]};
    polygon_setNormals(&side[2], 4, top_normals);

    // bottom side
    point_copy( &tv[0], &v[0] );
    point_copy( &tv[1], &v[1] );
    point_copy( &tv[2], &v[5] );
    point_copy( &tv[3], &v[4] );
    polygon_set( &side[3], 4, tv );
    Vector bottom_normals[4] = {normals[3], normals[3], normals[3], normals[3]};
    polygon_setNormals(&side[3], 4, bottom_normals);
    // left side
    point_copy( &tv[0], &v[0] );
    point_copy( &tv[1], &v[3] );
    point_copy( &tv[2], &v[7] );
    point_copy( &tv[3], &v[4] );
    polygon_set( &side[4], 4, tv );
    Vector left_normals[4] = {normals[4], normals[4], normals[4], normals[4]};
    polygon_setNormals(&side[4], 4, left_normals);
    // right side
    point_copy( &tv[0], &v[1] );
    point_copy( &tv[1], &v[2] );
    point_copy( &tv[2], &v[6] );
    point_copy( &tv[3], &v[5] );
    polygon_set( &side[5], 4, tv );
    Vector right_normals[4] = {normals[5], normals[5], normals[5], normals[5]};
    polygon_setNormals(&side[5], 4, right_normals);

    for(int i = 0; i < 6; i++){
        module_polygon(prism, &side[i]);
    }
    module_module(mod, prism);
}
