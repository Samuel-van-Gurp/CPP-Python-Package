#include "SnakeInterface.hpp"

SnakeInterface::SnakeInterface(const uint8_t *data, int width, int height, int stride, float alpha, float beta)
    : m_ImageHolder(ImageHolder<float>::StaticFactoryTypeChanger<uint8_t>(data, width, height, stride))
    , ImageProcessor()
    , m_contour(m_ImageHolder.getWidth() / 5, m_ImageHolder.getCenter(), 50)
    , m_engine(ImageProcessor, m_ImageHolder, m_contour, alpha, beta)
{
    ImageProcessor.PrepImage(m_ImageHolder);
    m_writer.saveImage(m_ImageHolder);
}

void SnakeInterface::run(int iterations)
{
    m_engine.RunSnake(iterations);
    m_writer.ContourOverLay(m_contour, m_ImageHolder, ImageProcessor);
}

