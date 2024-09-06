// Xi Guan
// CS5310 Lab3
// 5/29/2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"

// constructors and deconstructors
// allocate space for an image for the specified size unless either rows or cols is 0
Image *image_create(int rows, int cols){
    Image *image;
    if(rows == 0 || cols == 0){
        return NULL;
    }else{
        image = (Image*)malloc(rows * cols * sizeof(Image));
    }

    if(image == NULL){
        fprintf(stderr, "Error: Unable to allocate memory for image.\n");
        return NULL;
    }

    image_alloc(image, rows, cols);
    image->zBuffer = 1;
    image->a = 1;

    return image;
}

// given an uninitialized Image, set the rows and cols to zero, data to NULL
void image_init(Image *src){
    if(src == NULL){
        fprintf(stderr, "Error: Unable to initialize image.\n");
    }

    src -> rows = 0;
    src -> cols = 0;
    src -> data = NULL;
    src->zBuffer = 1;
    src->a = 1;
}

// allocate space for image data, 0.0 for RGB and 1.0 for A and Z
// return 0 is successful
// return non-zero if fails
// free existing memory if rows and cols are both non-zero
int image_alloc(Image *src, int rows, int cols){
    if(src == NULL){
        return -1;
    }

    // free existing memory if rows and cols are both non-zero
    image_dealloc(src);
    
    src -> rows = rows;
    src -> cols = cols;
    src->zBuffer = 1;
    src->a = 1;

    // allocate space for image data
    src -> data = (FPixel **)malloc(rows * sizeof(FPixel *));
    if(src -> data == NULL){
        return -1;
    }

    for (int i = 0; i < rows; i++) {
        src->data[i] = (FPixel *)malloc(cols * sizeof(FPixel));
        if (src->data[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(src->data[j]);
            }
            free(src->data);
            src->data = NULL;
            return -1;
        }
    }

    // Initialize pixel data to default values
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            src->data[i][j].c.c[0] = 0.0f; // Red
            src->data[i][j].c.c[1] = 0.0f; // Green
            src->data[i][j].c.c[2] = 0.0f; // Blue
            src->data[i][j].a = 1.0f;      // Alpha
            src->data[i][j].z = 1.0f;      // Depth
        }
    }

    return 0;
}

// de-allocate image data and reset the Image structure field
void image_dealloc(Image *src){
    if(src == NULL || src->data == NULL){
        return;
    }
    
    for (int i = 0; i < src -> rows; i++) {
        free(src->data[i]);
    }
    free(src -> data);
    src -> data = NULL;   
    src -> rows = 0;
    src -> cols = 0;
}

// deallocate image data and free the image structure
void image_free(Image *src){
    if(src == NULL) return;
    image_dealloc(src);
    free(src);
}

// I/O functions
// reads a PPM image from the given filename
// Initializes the alpha channel to 1.0 and the z channel to 1.0. 
// Returns a NULL pointer if the operation fails.
Image *image_read(char *filename){
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to open file for reading.\n");
        return NULL;
    }

    char format[3];
    int cols, rows, maxval;

    // Read the PPM header
    if (fscanf(fp, "%2s\n", format) != 1 || strcmp(format, "P6") != 0) {
        fprintf(stderr, "Error: Unsupported file format.\n");
        fclose(fp);
        return NULL;
    }

    // Read image dimensions and maximum color value
    if (fscanf(fp, "%d %d\n%d\n", &cols, &rows, &maxval) != 3) {
        fprintf(stderr, "Error: Invalid PPM header.\n");
        fclose(fp);
        return NULL;
    }

    // Allocate the Image structure
    Image *img = image_create(rows, cols);
    if (img == NULL) {
        fclose(fp);
        return NULL;
    }

    // Read pixel data
    unsigned char pixel[3];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (fread(pixel, sizeof(unsigned char), 3, fp) != 3) {
                fprintf(stderr, "Error: Unexpected end of file.\n");
                image_free(img);
                fclose(fp);
                return NULL;
            }
            img->data[i][j].c.c[0] = pixel[0] / 255.0f;
            img->data[i][j].c.c[1] = pixel[1] / 255.0f;
            img->data[i][j].c.c[2] = pixel[2] / 255.0f;
            img->data[i][j].a = 1.0f;
            img->data[i][j].z = 1.0f;
        }
    }
    
    fclose(fp);
    return img;
}

// writes a PPM image to the given filename
// Returns 0 on success.
int image_write(Image *src, char *filename){
     if (src == NULL || src->data == NULL) {
        return -1;
    }

    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Error: Unable to open file for writing.\n");
        return -1;
    }

    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", src->cols, src->rows);
    fprintf(fp, "255\n");

    unsigned char pixel[3];
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            pixel[0] = (unsigned char)((src->data[i][j].c.c[0] > 1.0 ? 1.0 : src->data[i][j].c.c[0]) * 255);
            pixel[1] = (unsigned char)((src->data[i][j].c.c[1] > 1.0 ? 1.0 : src->data[i][j].c.c[1]) * 255);
            pixel[2] = (unsigned char)((src->data[i][j].c.c[2] > 1.0 ? 1.0 : src->data[i][j].c.c[2]) * 255);
            fwrite(pixel, sizeof(unsigned char), 3, fp);
        }
    }

    fclose(fp);
    return 0;
}

// Access
// returns the FPixel at (r, c).
FPixel image_getf(Image *src, int r, int c){
    if (src == NULL || src->data == NULL || r < 0 || r >= src -> rows || c < 0 || c >= src -> cols) {
        FPixel error_pixel = {{{0.0f, 0.0f, 0.0f}}, 0.0f, 0.0f};
        return error_pixel;
    }
    return src -> data[r][c];
}

// returns the value of band b at pixel (r, c).
float image_getc(Image *src, int r, int c, int b){
    if (src == NULL || src->data == NULL || r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        return 0.0f; // Error value
    }
    switch (b) {
        case 0:
            return src->data[r][c].c.c[0];
        case 1:
            return src->data[r][c].c.c[1];
        case 2:
            return src->data[r][c].c.c[2];
        default:
            return 0.0f; // Error value
    }
}

// returns the alpha value at pixel (r, c).
float image_geta(Image *src, int r, int c){
    if (src == NULL || src->data == NULL || r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        return 0.0f; // Error value
    }
    return src->data[r][c].a;
}

// returns the depth value at pixel (r, c).
float image_getz(Image *src, int r, int c){
    if (src == NULL || src->data == NULL || r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        return 0.0f; // Error value
    }
    return src->data[r][c].z;
}

// sets the values of pixel (r, c) to the FPixel val.
void image_setf(Image *src, int r, int c, FPixel val){
    if (src == NULL || src->data == NULL || r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        return; // Error
    }
    src->data[r][c] = val;
}

// sets the value of pixel (r, c) band b to val.
void image_setc(Image *src, int r, int c, int b, float val){
    if (src == NULL || src->data == NULL || r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        return; // Error
    }
    switch (b) {
        case 0:
            src->data[r][c].c.c[0] = val;
            break;
        case 1:
            src->data[r][c].c.c[1] = val;
            break;
        case 2:
            src->data[r][c].c.c[2] = val;
            break;
        default:
            fprintf(stderr, "Error: Invalid band index.\n");
            break;
    }
}

// sets the alpha value of pixel (r, c) to val.
void image_seta(Image *src, int r, int c, float val){
    if (src == NULL || src->data == NULL || r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        return; // Error
    }
    src->data[r][c].a = val;
}

// sets the depth value of pixel (r, c) to val.
void image_setz(Image *src, int r, int c, float val){
    if (src == NULL || src->data == NULL || r < 0 || r >= src->rows || c < 0 || c >= src->cols) {
        return; // Error
    }
    src->data[r][c].z = val;
}

//Utility
// resets every pixel to a default value (e.g. Black, alpha value of 1.0, z value of 1.0).
void image_reset(Image *src){
    if (src == NULL || src->data == NULL) return;

    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            src->data[i][j].c.c[0] = 0.0f; // Red
            src->data[i][j].c.c[1] = 0.0f; // Green
            src->data[i][j].c.c[2] = 0.0f; // Blue
            src->data[i][j].a = 1.0f;      // Alpha
            src->data[i][j].z = 1.0f;      // Depth
        }
    }
}

// sets every FPixel to the given value
void image_fill(Image *src, FPixel val){
    if (src == NULL || src->data == NULL) return;

    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            src->data[i][j] = val;
        }
    }
}

//  sets the (r, g, b) val ues of each pixel to the given color.
void image_fillrgb(Image *src, float r, float g, float b){
    if (src == NULL || src->data == NULL) return;

    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            src->data[i][j].c.c[0] = r;
            src->data[i][j].c.c[1] = g;
            src->data[i][j].c.c[2] = b;
        }
    }
}

// set the alpha value of each pixel to the given value
void image_filla(Image *src, float a){
    if (src == NULL || src->data == NULL) return;

    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            src->data[i][j].a = a;
        }
    }
}

// set the depth value of each pixel to the given value
void image_fillz(Image *src, float z){
    if (src == NULL || src->data == NULL) return;

    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            src->data[i][j].z = z;
        }
    }
}

// copy the Color data to the proper pixel
void image_setColor(Image *src, int r, int c, Color val){
    FPixel *pixel = &src->data[r][c];
    color_copy(&(pixel->c), &val);
    pixel->a = 1.0;
}

// return a color structure built from the pixel values
Color image_getColor(Image *src, int r, int c){
    FPixel p = image_getf(src, r, c);
    return p.c;
}