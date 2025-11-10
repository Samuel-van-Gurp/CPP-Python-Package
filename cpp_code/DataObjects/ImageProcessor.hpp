#ifndef CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGE_HPP
#define CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGE_HPP

#include "Contour.hpp"
#include "Point.hpp"
#include "ImageHolder.hpp"
#include <cstdint>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

enum class BlurType { Small, Medium, Large };

class ImageProcessor
{
    public:    
    ImageProcessor();
    void PrepImage(ImageHolder<float> &image);
    void normaliseImageIntensity(ImageHolder<float> &img);
    void scaleIntensity(int factor, ImageHolder<float> &image) const;

    void invertImageIntensity(ImageHolder<float> &image) const;

    Point GetCoordinateOfMaximumNeighborValue(const Point &p, const ImageHolder<float> &image) const;
    std::vector<std::vector<float>> getNeighbourhood(const Point &p, const ImageHolder<float> &image) const;

    ImageHolder<float> ConvolveImage(const std::vector<std::vector<float>> &kernel, const ImageHolder<float> &image) const;

    ImageHolder<float> ComputeGradientMagnitude(const ImageHolder<float> &image);

    void BlurImage(BlurType blurType, ImageHolder<float> &image);

private:

};

#endif // CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGE_HPP