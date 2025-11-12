#include "api_impl.hpp"
#include "DataObjects/ImageProcessor.hpp"
#include "DataObjects/Contour.hpp"
#include "DataObjects/Point.hpp"
#include "IO/WriteImage.hpp"
#include "Algorithm/SnakeEngine.hpp"
#include "SnakeInterface.hpp"

SnakeInterface* setupSnake(ImageHolder<float>* imageHolder_ptr, float alpha, float beta, float iterations, float contour_center_x, float contour_center_y, float contour_radius, int contour_points)
{
    // construct classed needed for SnakeInterface
    ImageProcessor imageProcessor;

    Contour contour(contour_radius, Point(contour_center_x, contour_center_y), contour_points);
    SnakeEngine engine(imageProcessor, *imageHolder_ptr, contour, alpha, beta); 

    SnakeInterface* snakeInterface = new SnakeInterface(*imageHolder_ptr, imageProcessor, contour, alpha, beta);
    return snakeInterface;
}

ImageHolder<float>* constructImage(const uint8_t * data, int width, int height, int stride)
{
    return new ImageHolder<float>(ImageHolder<float>::StaticFactoryTypeChanger(data, width, height, stride));
}
