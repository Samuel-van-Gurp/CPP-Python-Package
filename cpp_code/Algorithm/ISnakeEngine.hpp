#ifndef ISNAKEENGINE_HPP
#define ISNAKEENGINE_HPP
#include "DataObjects/Point.hpp"
#include "DataObjects/Contour.hpp"
#include "ContourResampler.hpp"
#include <iostream>

class ISnakeEngine
{
    public:
        ISnakeEngine() {}
        virtual ~ISnakeEngine() = default;
        std::vector<Point> RunSnake(int iterations, Contour &contour);
        virtual Point getNextStep(int index, Point &p, const Contour &contour) = 0;
        static constexpr int STOP_CRITERION = 5;


        bool EvolveContour(Contour &m_contour);
        int hasPointMoved(int iterationsPerRound, const Point &oldPoint, const Point &newPoint);
};

#endif // ISNAKEENGINE_HPP