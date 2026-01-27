#ifndef SNAKECONTROLLER_HPP
#define SNAKECONTROLLER_HPP

#include <cstdint>
#include "ImageProcessing/ImageProcessorFacade.hpp"
#include "DataObjects/Contour.hpp"
#include "Algorithm/ISnakeEngine.hpp"
#include "MagicNumbers.hpp"
#include "IO/WriteImage.hpp"
#include "DataObjects/ImageHolder.hpp"
#include "DataObjects/SnakeParams.hpp"
#include "ImageProcessing/NaiveConvolve.hpp"
#include "ImageProcessing/IntensityManipulator.hpp"
#include <memory>

class SnakeController
{
public:

    SnakeController(ImageHolder<float> imageHolder, std::unique_ptr<ImageProcessorFacade> imageProcessor, Contour contour, std::unique_ptr<ISnakeEngine> engine);

    std::vector<Point> run(int iterations);

    static SnakeController createSnakeController(ImageHolder<float> imageHolder, SnakeParams snakeParams, SnakeSolver solver);

private:
    ImageHolder<float> m_imageHolder;
    std::unique_ptr<ImageProcessorFacade> m_imageProcessor;
    Contour m_contour;
    std::unique_ptr<ISnakeEngine> m_engine;
    WriteImage m_writer;
};

#endif // SNAKECONTROLLER_HPP