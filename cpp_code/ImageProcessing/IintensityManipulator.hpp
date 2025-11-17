
#ifndef IINTENSITYMANIPULATOR_HPP
#define IINTENSITYMANIPULATOR_HPP

#include "DataObjects/ImageHolder.hpp"

class IIntensityManipulator
{
public:
    virtual ~IIntensityManipulator() = default;
    virtual void invertImageIntensity(ImageHolder<float> &image) const = 0;
    virtual void scaleIntensity(int factor, ImageHolder<float> &image) const = 0;
    virtual void normaliseImageIntensity(ImageHolder<float>& img) const = 0;
};

#endif // IINTENSITYMANIPULATOR_HPP