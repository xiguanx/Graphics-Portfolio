#ifndef FPIXEL_H
#define FPIXEL_H
#include "color.h"

typedef struct {
  Color c;
  float a;
  float z;
} FPixel;

#endif