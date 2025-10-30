#include "api_impl.hpp"
#include "DataObjects/Image.hpp"
#include "DataObjects/Contour.hpp"
#include "DataObjects/Point.hpp"
#include "IO/WriteImage.hpp"
#include "Algorithm/SnakeEngine.hpp"

int hay = 42;

int getHay() {
    return testFunction();
}

void constructImage(const uint8_t *data, int width, int height, int stride)
{
    WriteImage writer;
    Image img(data, width, height, stride);
    writer.saveImage(img);
    std::cout << "Image processed and saved as output.pgm\n";

    Contour contour(img.GetWidth() / 3, img.GetCenter(), 50);
    // writer.ContourOverLay(contour, img);
    SnakeEngine engine(img, contour);
    engine.RunSnake(100);
    writer.ContourOverLay(contour, img);

}

int testFunction()
{
    return 123;
}
