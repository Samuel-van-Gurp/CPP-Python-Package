#pragma once

#include <cstdint>
#include <vector>
#include "cpp_code/DataObjects/ImageProcessor.hpp"
#include "cpp_code/SnakeInterface.hpp"

#ifdef __cplusplus
extern "C" {
#endif

SnakeInterface* setupSnake(ImageHolder<float> *imageHolder_ptr,
                            float alpha, 
                            float beta, 
                            float iterations, 
                            float contour_center_x, 
                            float contour_center_y, 
                            float contour_radius, 
                            int contour_points);

ImageHolder<float> *constructImage(const uint8_t *data, int width, int height, int stride);
#ifdef __cplusplus
}
#endif

