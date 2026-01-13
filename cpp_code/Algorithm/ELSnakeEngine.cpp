#include "ELSnakeEngine.hpp"

ELSnakeEngine::ELSnakeEngine(ImageProcessorFacade &imageProcessor, ImageHolder<float> &imageHolder, Contour &contour, float alpha, float beta)
    : m_imageProcessor(imageProcessor), m_imageHolder(imageHolder), m_contour(contour), alpha(alpha), beta(beta)
{
    
    m_gradients = m_imageProcessor.PrepareImageForELSnake(m_imageHolder);
}

std::tuple<float, float> ELSnakeEngine::combineForces(const std::tuple<float, float>& internalForce, const std::tuple<float, float>& externalForce)
{
    std::tuple<float, float> combinedForce;
    std::get<0>(combinedForce) = std::get<0>(internalForce)*INTERNAL_FORCE_SCALE - std::get<0>(externalForce);
    std::get<1>(combinedForce) = std::get<1>(internalForce)*INTERNAL_FORCE_SCALE - std::get<1>(externalForce);
    return combinedForce;
}

Point ELSnakeEngine::getNextStep(int index, Point& p)
{
    const float stepSize = 0.1f;
    const float maxDisp = 1.0f;

    auto internalForce = getInternalForce(index, m_contour);
    auto externalForce = getExternalForce(p); 

    float new_x = p.X + std::clamp(stepSize * std::get<0>(combineForces(internalForce, externalForce)), -maxDisp, maxDisp);
    float new_y = p.Y + std::clamp(stepSize * std::get<1>(combineForces(internalForce, externalForce)), -maxDisp, maxDisp);

    return Point(new_x,new_y);
}

std::tuple<float, float> ELSnakeEngine::getExternalForce(const Point &p)
{
    float fx = m_gradients->gx.getPixel(p);
    float fy = m_gradients->gy.getPixel(p);
    return std::make_tuple(fx, fy);
}

std::tuple<float, float> ELSnakeEngine::getInternalForce(int ContourIndex, const Contour& contour)
{
    std::tuple<float, float> tentionForce = contour.secondDiff(ContourIndex);
    std::tuple<float, float> CurveForce = contour.fourthDiff(ContourIndex); 

    // Combine forces
    std::tuple<float, float> internalForce;
    
    get<0>(internalForce) = (alpha * get<0>(tentionForce) - beta * get<0>(CurveForce)); 
    get<1>(internalForce) = (alpha * get<1>(tentionForce) - beta * get<1>(CurveForce));

    return internalForce;
}