#include "SnakeInterface.hpp"

SnakeInterface::SnakeInterface(const uint8_t *data, int width, int height, int stride, float alpha, float beta)
    : m_img(data, width, height, stride)
    , m_contour(m_img.GetWidth() / 5, m_img.GetCenter(), 100)
    , m_engine(m_img, m_contour, alpha, beta)
{
    m_img.PrepImage();
    m_writer.saveImage(m_img);
}

void SnakeInterface::run(int iterations)
{
    m_engine.RunSnake(iterations);
    m_writer.ContourOverLay(m_contour, m_img);
}
