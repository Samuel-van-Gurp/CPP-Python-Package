#pragma once
#include <stdint.h>
#include "cpp_code/SnakeInterface.hpp"

#ifdef __cplusplus // if this is compiled by a C++ compiler
  extern "C" { // tell the C++ compiler to treat the following code as C code
#endif

#ifdef _WIN32
  #ifdef BUILDING_API
    #define EXPORT_API __declspec(dllexport)
  #else
    #define EXPORT_API __declspec(dllimport)
  #endif
#else
  #define EXPORT_API
#endif

typedef struct 
{
    float alpha;
    float beta;
    int iterations;
    float contour_center_x;
    float contour_center_y;
    float contour_radius;
    int contour_points;
} SnakeParams;

typedef struct 
{
  uint8_t* data;
  int width;
  int height;
  int stride;
} ImageInfo;

EXPORT_API void runSnake(ImageInfo const* imageInfo, SnakeParams const* params, Point* outContour_ptr, int* outContourLength_ptr);
#ifdef __cplusplus
}
#endif

