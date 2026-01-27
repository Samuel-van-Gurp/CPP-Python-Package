#include "ISnakeEngine.hpp"
#include <cmath>

std::vector<Point> ISnakeEngine::RunSnake(int iterations, Contour &contour)
{
    for (int i = 0; i < iterations; ++i)
    {
        if (!EvolveContour(contour)) {
            break; 
        }
    }
    return contour.getContourPoints();
}

bool ISnakeEngine::EvolveContour(Contour &contour)
{
    // copy current contour to new contour
    Contour newContour = contour;
    int iterationsPerRound = 0;
    int contourSize = contour.Size();
    
    for (int i = 0; i < contourSize; ++i)
    {
        Point& p = contour[i];
        newContour[i] = getNextStep(i, p, contour);
        iterationsPerRound = hasPointMoved(iterationsPerRound, p, newContour[i]);
    }

    if (iterationsPerRound < STOP_CRITERION)
    {
        contour = std::move(newContour);
        return false;
    }
    
    auto resampledContour = ContourResampler::resampleContour(newContour);

    // move new contour to the old contour
    contour = std::move(resampledContour);
    return true;
}

int ISnakeEngine::hasPointMoved(int iterationsPerRound, const Point & oldPoint, const Point & newPoint)
{
    const float moveEpsilon = 1e-3f;
    float dx = newPoint.X - oldPoint.X;
    float dy = newPoint.Y - oldPoint.Y;
    if (std::sqrt(dx*dx + dy*dy) > moveEpsilon)
    {
        iterationsPerRound++;
    }
    return iterationsPerRound;
}