#include "api_impl.hpp"
#include "DataObjects/ImageProcessor.hpp"
#include "DataObjects/Contour.hpp"
#include "DataObjects/Point.hpp"
#include "IO/WriteImage.hpp"
#include "Algorithm/SnakeEngine.hpp"
#include "SnakeInterface.hpp"

int hay = 42;

int getHay() {
    return testFunction();
}

void constructImage(const uint8_t *data, int width, int height, int stride)
{
    SnakeInterface snakeInterface(data, width, height, stride, 1.0f, 0.01f);
    snakeInterface.run(100);
}

int testFunction()
{
    return 123;
}


