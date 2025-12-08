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
        if (nextStep.X != p.X || nextStep.Y != p.Y)
        {
            iterationsPerRound++;
        }

    }
    std::cout << "Points moved in this iteration: " << iterationsPerRound << "\n";
    if (iterationsPerRound < stopCriterion)
    {
        std::cout << "No points moved in this iteration. Stopping evolution.\n";
        contour = std::move(newContour);
        return false;
    }
    // move new contour to the old contour
    contour = std::move(newContour);
    return true;
}