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

    // todo, it is realy crappy to have two different prepare functions for the different snake types, this should be orcestrated from the snake engine class i think
    void PrepareImageForGreedySnake(ImageHolder<float> &image);

    void PrepareImageForELSnake(ImageHolder<float> &image);

    void normaliseImageIntensity(ImageHolder<float> &img);
    void scaleIntensity(int factor, ImageHolder<float> &image) const;
    void invertImageIntensity(ImageHolder<float> &image) const;

    ImageHolder<float> CalculateGradientX(const ImageHolder<float> &image) const;

    ImageHolder<float> CalculateGradientY(const ImageHolder<float> &image) const;

    // ImageHolder<float> ConvolveImage(const std::vector<std::vector<float>> &kernel, const ImageHolder<float> &image) const;

    ImageHolder<float> ComputeGradientMagnitude(const ImageHolder<float> &image);

    void BlurImage(BlurType blurType, ImageHolder<float> &image);

private:

    static const std::vector<std::vector<float>> SobelX;
    static const std::vector<std::vector<float>> SobelY;

    std::unique_ptr<IConvolver> m_convolver;
    std::unique_ptr<IIntensityManipulator> m_intensityManipulator;
};

#endif // CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGE_HPP