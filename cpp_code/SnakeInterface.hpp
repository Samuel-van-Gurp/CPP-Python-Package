#ifndef SNAKEINTERFACE_HPP
#define SNAKEINTERFACE_HPP

#include <cstdint>
#include <cassert>
#include "DataObjects/ImageProcessor.hpp"
#include "DataObjects/Contour.hpp"
#include "Algorithm/GreedySnakeEngine.hpp"
#include "IO/WriteImage.hpp"
#include "DataObjects/ImageHolder.hpp"

class SnakeController
{
public:
    SnakeController(ImageHolder<float> imageHolder, ImageProcessor imageProcessor, Contour contour, float alpha, float beta);
    void run(int iterations);

private:
    ImageHolder<float> m_imageHolder;
    ImageProcessor m_imageProcessor;
    Contour m_contour;
    GreedySnakeEngine m_engine;
    WriteImage m_writer;
};

#endif // SNAKEINTERFACE_HPP