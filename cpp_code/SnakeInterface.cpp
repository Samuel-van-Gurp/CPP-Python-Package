#include "SnakeInterface.hpp"

SnakeInterface::SnakeInterface(const uint8_t *data, int width, int height, int stride)
{

    WriteImage writer;
    Image img(data, width, height, stride);
    img.PrepImage();
    writer.saveImage(img);
    std::cout << "Image processed and saved as output.pgm\n";

    Contour contour(img.GetWidth() / 5, img.GetCenter(), 50);
    // writer.ContourOverLay(contour, img);
    SnakeEngine engine(img, contour, 1.0f, 0.01f);
    engine.RunSnake(400);

    writer.ContourOverLay(contour, img);
}
