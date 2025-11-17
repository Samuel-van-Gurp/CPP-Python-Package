#ifndef INTENSITYMANIPULATOR_HPP
#define INTENSITYMANIPULATOR_HPP

#include "DataObjects/ImageHolder.hpp"
#include "IIntensityManipulator.hpp"
    
class IntensityManipulator : public IIntensityManipulator
{
    public:
        IntensityManipulator();
        void invertImageIntensity(ImageHolder<float> &image) const;
        void normaliseImageIntensity(ImageHolder<float> &img) const;
        void scaleIntensity(int factor, ImageHolder<float> &image) const;

    private:
};

#endif // INTENSITYMANIPULATOR_HPP
