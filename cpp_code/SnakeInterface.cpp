#include "SnakeInterface.hpp"

SnakeController::SnakeController(ImageHolder<float> imageHolder, ImageProcessor& imageProcessor, Contour contour, float alpha, float beta)
    : m_imageHolder(imageHolder)
    , m_imageProcessor(imageProcessor)
    , m_contour(contour)
    , m_engine(GreedySnakeEngine(m_imageProcessor, m_imageHolder, m_contour, alpha, beta))
{
    m_imageProcessor.PrepImage(m_imageHolder);
    m_writer.saveImage(m_imageHolder);
}

void SnakeController::run(int iterations)
{
    m_engine.RunSnake(iterations);
    m_writer.ContourOverLay(m_contour, m_imageHolder, m_imageProcessor);
}

