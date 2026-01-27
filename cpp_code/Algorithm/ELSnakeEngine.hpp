#ifndef ELSAKESNAKEENGINE_HPP
#define ELSAKESNAKEENGINE_HPP

#include <vector>
#include <iostream>
#include <tuple>
#include <algorithm>
#include <optional>
#include "Algorithm/ISnakeEngine.hpp"
#include "DataObjects/Point.hpp"
#include "Algorithm/ELSnakeEngine.hpp"
#include "ImageProcessing/ImageProcessorFacade.hpp"


class ELSnakeEngine : public ISnakeEngine
{
public:

    ELSnakeEngine(ImageProcessorFacade::Gradients gradients, float alpha, float beta);

    std::tuple<float, float> combineForces(const std::tuple<float, float> &internalForce, const std::tuple<float, float> &externalForce);

    Point getNextStep(int index, Point &p, const Contour &contour);

    std::tuple<float, float> getExternalForce(const Point &p);

    std::tuple<float, float> getInternalForce(int ContourIndex, const Contour &contour);

private:
    static constexpr float INTERNAL_FORCE_SCALE = 100.0f; // Scale factor to match external force magnitude
    // std::optional<ImageProcessorFacade::Gradients> m_gradients;
    ImageProcessorFacade::Gradients m_gradients;

    const float alpha;    
    const float beta;
};

#endif ELSAKESNAKEENGINE_HPP
