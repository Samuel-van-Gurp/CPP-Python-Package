#ifndef ELSAKESNAKEENGINE_HPP
#define ELSAKESNAKEENGINE_HPP

#include <vector>
#include <iostream>
#include <tuple>
#include "Algorithm/ISnakeEngine.hpp"
#include "DataObjects/Point.hpp"
#include "Algorithm/ELSnakeEngine.hpp"
#include "DataObjects/Contour.hpp"
#include "ImageProcessing/ImageProcessorFacade.hpp"
#include "DataObjects/Contour.hpp"
#include "Algorithm/ISnakeEngine.hpp"


class ELSnakeEngine : public ISnakeEngine
{
    public:

    ELSnakeEngine(const ImageProcessorFacade & image, const ImageHolder<float>& imageHolder, Contour & contour, float alpha, float beta);

    Point* RunSnake(int iterations);

    private:
    bool EvolveContour();

    Point getNextStep(int index, Point &p);

    std::tuple<float, float> getInternalForce(const Point &p);

    std::tuple<float, float> getExternalForce(int ContourIndex);

private:

    static constexpr int stopCriterion = 5;

    Contour m_contour;
    ImageHolder<float> m_imageHolder;
    const ImageProcessorFacade &m_imageProcessor;

    const float alpha;    
    const float beta;

    
};

#endif ELSAKESNAKEENGINE_HPP
