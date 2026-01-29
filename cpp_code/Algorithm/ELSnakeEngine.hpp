#ifndef ELSAKESNAKEENGINE_HPP
#define ELSAKESNAKEENGINE_HPP

#include <tuple>
#include "Algorithm/ISnakeEngine.hpp"
#include "DataObjects/Point.hpp"
#include "ImageProcessing/ImageProcessorFacade.hpp"


class ELSnakeEngine : public ISnakeEngine
{
public:
    static constexpr float INTERNAL_FORCE_SCALE = 100.0f; // Scale factor to match external force magnitude

    ELSnakeEngine(ImageProcessorFacade::Gradients gradients, float alpha, float beta);

    std::tuple<float, float> combineForces(const std::tuple<float, float> &internalForce, const std::tuple<float, float> &externalForce);

    Point getNextStep(int index, Point &p, const Contour &contour);

    private:

    std::tuple<float, float> getExternalForce(const Point &p);

    std::tuple<float, float> getInternalForce(int ContourIndex, const Contour &contour);
    
    ImageProcessorFacade::Gradients m_gradients;
    
    const float alpha;    
    const float beta;
};

#endif // ELSAKESNAKEENGINE_HPP
