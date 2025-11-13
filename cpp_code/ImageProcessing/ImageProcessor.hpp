#ifndef CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGE_HPP
#define CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGE_HPP

#include "DataObjects/Contour.hpp"
#include "DataObjects/Point.hpp"
#include "DataObjects/ImageHolder.hpp"
#include "ImageProcessing/IConvolve.hpp"
#include <cstdint>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <memory>

enum class BlurType { Small, Medium, Large };

class ImageProcessor
{
    public:    

    ImageProcessor(std::unique_ptr<IConvolver> convolver);
    void PrepImage(ImageHolder<float> &image);
    void normaliseImageIntensity(ImageHolder<float> &img);
    void scaleIntensity(int factor, ImageHolder<float> &image) const;

    void invertImageIntensity(ImageHolder<float> &image) const;

    Point GetCoordinateOfMaximumNeighborValue(const Point &p, const ImageHolder<float> &image) const;

    ImageHolder<float> ConvolveImage(const std::vector<std::vector<float>> &kernel, const ImageHolder<float> &image) const;

    ImageHolder<float> ComputeGradientMagnitude(const ImageHolder<float> &image);

    void BlurImage(BlurType blurType, ImageHolder<float> &image);

private:
    std::unique_ptr<IConvolver> m_convolver;
};

#endif // CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGE_HPP