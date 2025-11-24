#ifndef CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGE_HPP
#define CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGE_HPP

#include "DataObjects/ImageHolder.hpp"
#include "ImageProcessing/IConvolve.hpp"
#include "ImageProcessing/IIntensityManipulator.hpp"

#include <cstdint>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <memory>

enum class BlurType { Small, Medium, Large };

class ImageProcessorFacade
{
    public:    

    ImageProcessorFacade(std::unique_ptr<IConvolver> convolver, std::unique_ptr<IIntensityManipulator> intensityManipulator);

    void PrepImage(ImageHolder<float> &image);
    
    void normaliseImageIntensity(ImageHolder<float> &img);
    void scaleIntensity(int factor, ImageHolder<float> &image) const;
    void invertImageIntensity(ImageHolder<float> &image) const;

    // ImageHolder<float> ConvolveImage(const std::vector<std::vector<float>> &kernel, const ImageHolder<float> &image) const;

    ImageHolder<float> ComputeGradientMagnitude(const ImageHolder<float> &image);

    void BlurImage(BlurType blurType, ImageHolder<float> &image);

private:
    std::unique_ptr<IConvolver> m_convolver;
    std::unique_ptr<IIntensityManipulator> m_intensityManipulator;
};

#endif // CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGE_HPP