#include "ISnakeEngine.hpp"

Point *ISnakeEngine::RunSnake(int iterations, Contour &contour)
{
    for (int i = 0; i < iterations; ++i)
    {
        if (!EvolveContour(contour)) {
            std::cout << "Contour stabilized after " << i << " iterations.\n";
            break; 
        }
    }
    std::cout << "Cut-off iterations reached.\n";
    return contour.getContourPoints_ptr();
}

bool ISnakeEngine::EvolveContour(Contour &contour)
{
    //copy current contour to new contour
    Contour newContour = contour;
    int iterationsPerRound = 0;

    int contourSize = contour.Size();
    for (int i = 0; i < contourSize; ++i)
    {
        Point& p = contour[i];

        Point nextStep = getNextStep(i, p);
        newContour[i] = nextStep;
        
        // check if point has moved
        const float moveEpsilon = 1e-3f;
        float dx = nextStep.X - p.X;
        float dy = nextStep.Y - p.Y;
        if (std::sqrt(dx*dx + dy*dy) > moveEpsilon)
        {
            iterationsPerRound++;
        }

    }
    if (iterationsPerRound < stopCriterion)
    {
        contour = std::move(newContour);
        return false;
    }
    
    auto resampledContour = ContourResampler::resampleContour(newContour);

    // move new contour to the old contour
    contour = std::move(resampledContour);

    return true;
}
