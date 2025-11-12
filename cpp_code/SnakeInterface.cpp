#include "SnakeInterface.hpp"

SnakeInterface::SnakeInterface(ImageHolder<float> imageHolder, ImageProcessor imageProcessor, Contour contour, float alpha, float beta)
    : m_imageHolder(imageHolder)
    , m_imageProcessor(imageProcessor)
    , m_contour(contour)
    , m_engine(SnakeEngine(m_imageProcessor, m_imageHolder, m_contour, alpha, beta))
{
    m_imageProcessor.PrepImage(m_imageHolder);
    m_writer.saveImage(m_imageHolder);
}

void SnakeInterface::run(int iterations)
{
    m_engine.RunSnake(iterations);
    m_writer.ContourOverLay(m_contour, m_imageHolder, m_imageProcessor);
}

