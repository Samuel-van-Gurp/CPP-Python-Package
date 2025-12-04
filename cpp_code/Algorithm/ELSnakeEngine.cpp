#include "ELSnakeEngine.hpp"
#include <iomanip> // todo remove

ELSnakeEngine::ELSnakeEngine(const ImageProcessorFacade &imageProcessor, const ImageHolder<float> &imageHolder, Contour &contour, float alpha, float beta)
    : m_imageProcessor(imageProcessor), m_imageHolder(imageHolder), m_contour(contour), alpha(alpha), beta(beta)
{

}

Point* ELSnakeEngine::RunSnake(int iterations)
{
    for (int i = 0; i < iterations; ++i)
    {
        if (!EvolveContour()) {
            std::cout << "Contour stabilized after " << i << " iterations.\n";
            break; 
        }
    }

    return m_contour.getContourPoints_ptr();
}

bool ELSnakeEngine::EvolveContour()
{
    //copy current contour to new contour
    Contour newContour = m_contour;
    int iterationsPerRound = 0;

    int contourSize = m_contour.Size();
    for (int i = 0; i < contourSize; ++i)
    {
        Point& p = m_contour[i];

        Point nextStep = getNextStep(i, p);
        newContour[i] = nextStep;
        
        // check if point has moved
        if (nextStep.X != p.X || nextStep.Y != p.Y)
        {
            iterationsPerRound++;
        }

    }
    std::cout << "Points moved in this iteration: " << iterationsPerRound << "\n";
    if (iterationsPerRound == stopCriterion)
    {
        std::cout << "No points moved in this iteration. Stopping evolution.\n";
        m_contour = std::move(newContour);
        return false;
    }
    // move new contour to the old contour
    m_contour = std::move(newContour);
    return true;
}

Point ELSnakeEngine::getNextStep(int index, Point& p)
{
    float stepSize = 0.10f;

    auto internalForce = getInternalForce(index);
    auto externalForce = getExternalForce(p);

    // todo remove
    // std::cout<<"internalForce" << get<0>(internalForce) << "," << get<1>(internalForce) << "\n";
    // std::cout<<"externalForce" << get<0>(externalForce) << "," << get<1>(externalForce)<< "\n";

    float new_x = p.X + stepSize * (get<0>(internalForce) + get<0>(externalForce));
    float new_y = p.Y + stepSize * (get<1>(internalForce) + get<1>(externalForce));
    // print external and internal forces
    std::cout << "externalForce" << get<0>(externalForce) <<"," << get<1>(externalForce)<< "\n";; 
    std::cout << " internalForce" << get<0>(internalForce) <<"," << get<1>(internalForce) << "\n";
    // Print old and new point
    std::cout << "Point " << index << ": (" << p.X << ", " << p.Y << ") -> (" << new_x << ", " << new_y << ")\n";    

    return Point(new_x,new_y);
}

std::tuple<float, float> ELSnakeEngine::getExternalForce(const Point &p)
{
    float fx = m_imageProcessor.CalculateGradientX(m_imageHolder).getPixel(p);
    float fy = m_imageProcessor.CalculateGradientY(m_imageHolder).getPixel(p);

    return std::make_tuple(fx, fy);
}

std::tuple<float, float> ELSnakeEngine::getInternalForce(int ContourIndex)
{
    std::tuple<float, float> tentionForce = m_contour.secondDiff(ContourIndex);
    std::tuple<float, float> CurveForce = m_contour.fourthdDiff(ContourIndex); 

    // Combine forces
    std::tuple<float, float> internalForce;
    
    get<0>(internalForce) = alpha * get<0>(tentionForce) + beta * get<0>(CurveForce);
    get<1>(internalForce) = alpha * get<1>(tentionForce) + beta * get<1>(CurveForce);


    return internalForce;
}