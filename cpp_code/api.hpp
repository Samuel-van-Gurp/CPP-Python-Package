#pragma once
#include <stdint.h>
#include <vector>
#include "cpp_code/api_impl.hpp"
#include "DataObjects/SnakeParams.hpp"
#include "DataObjects/ImageInfo.hpp"

std::vector<Point> runSnake(ImageInfo const* imageInfo, SnakeParams params);

