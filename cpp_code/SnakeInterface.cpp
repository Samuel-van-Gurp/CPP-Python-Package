#include "SnakeInterface.hpp"

SnakeController::SnakeController(ImageHolder<float> imageHolder, std::unique_ptr<ImageProcessorFacade> imageProcessor, Contour contour, std::unique_ptr<ISnakeEngine> engine, float alpha, float beta)
    : m_imageHolder(imageHolder)
    , m_imageProcessor(std::move(imageProcessor))
    , m_contour(contour)
    , m_engine(std::move(engine))
{
    
    m_engine = std::make_unique<GreedySnakeEngine>(*m_imageProcessor, m_imageHolder, m_contour, alpha, beta);

    m_imageProcessor->PrepImage(m_imageHolder);
    m_writer.saveImage(m_imageHolder);
}

Point* SnakeController::run(int iterations, int* out_count_ptr)
{
    Point* contourPoints = m_engine->RunSnake(iterations);
    m_writer.ContourOverLay(m_contour, m_imageHolder, *m_imageProcessor);
    
    if (out_count_ptr) 
    {
        *out_count_ptr = m_contour.Size();
    }
    return contourPoints;
}

