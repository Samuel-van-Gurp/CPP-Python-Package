#include "api_impl.hpp"
#include "ImageProcessing/ImageProcessor.hpp"
#include "ImageProcessing/NaiveConvolve.hpp"
#include "DataObjects/Contour.hpp"
#include "DataObjects/Point.hpp"
#include "IO/WriteImage.hpp"
#include "Algorithm/GreedySnakeEngine.hpp"
#include "SnakeInterface.hpp"

#include <memory>

SnakeController* setupSnake(ImageHolder<float>* imageHolder_ptr, float alpha, float beta, float iterations, float contour_center_x, float contour_center_y, float contour_radius, int contour_points)
{
    auto imageProcessor = std::make_unique<ImageProcessor>(std::make_unique<NaiveConvolve>());

    Contour contour(contour_radius, Point(contour_center_x, contour_center_y), contour_points);
    std::unique_ptr<ISnakeEngine> engine = std::make_unique<GreedySnakeEngine>(*imageProcessor, *imageHolder_ptr, contour, alpha, beta);

    SnakeController* snakeInterface = new SnakeController(*imageHolder_ptr, std::move(imageProcessor), contour, std::move(engine), alpha, beta);
    return snakeInterface;
}

ImageHolder<float>* constructImage(const uint8_t * data, int width, int height, int stride)
{
    return new ImageHolder<float>(ImageHolder<float>::StaticFactoryTypeChanger(data, width, height, stride));
}
