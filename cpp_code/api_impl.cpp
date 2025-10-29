#include "api_impl.hpp"

int hay = 42;

int getHay() {
    return testFunction();
}

void constructImage(const uint8_t *data, int width, int height, int stride)
{
    Image img(data, width, height, stride);
    img.SaveAsPGM();
    std::cout << "Image processed and saved as output.pgm\n";   
}

int testFunction()
{
    return 123;
}
