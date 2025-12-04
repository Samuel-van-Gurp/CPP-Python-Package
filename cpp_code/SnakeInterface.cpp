#include "SnakeInterface.hpp"
#include "Algorithm/GreedySnakeEngine.hpp"
#include "Algorithm/ELSnakeEngine.hpp"
#include <memory>

SnakeController::SnakeController(ImageHolder<float> imageHolder, std::unique_ptr<ImageProcessorFacade> imageProcessor, Contour contour, std::unique_ptr<ISnakeEngine> engine, float alpha, float beta)
    : m_imageHolder(imageHolder)
    , m_imageProcessor(std::move(imageProcessor))
    , m_contour(contour)
    , m_engine(std::move(engine))
{
    
    // m_imageProcessor->PrepareImageForGreedySnake(m_imageHolder);
    // m_engine = std::make_unique<GreedySnakeEngine>(*m_imageProcessor, m_imageHolder, m_contour, alpha, beta);
    m_imageProcessor->PrepareImageForELSnake(m_imageHolder);
    m_engine = std::make_unique<ELSnakeEngine>(*m_imageProcessor, m_imageHolder, m_contour, alpha, beta);

}

Point* SnakeController::run(int iterations, int* out_count_ptr)
{
    Point* contourPoints = m_engine->RunSnake(iterations);    
    if (out_count_ptr) 
    {
        *out_count_ptr = m_contour.Size();
    }
    return contourPoints;
}

