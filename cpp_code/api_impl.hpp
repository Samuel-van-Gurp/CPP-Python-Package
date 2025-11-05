#pragma once

#include <cstdint>
#include <vector>
#include "cpp_code/DataObjects/Image.hpp"

#ifdef __cplusplus
extern "C" {
#endif

int getHay(void);

void constructImage(const uint8_t* data, int width, int height, int stride);

#ifdef __cplusplus
}
#endif

int testFunction();