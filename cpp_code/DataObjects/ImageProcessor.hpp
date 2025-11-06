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

class ImageProcessor
{
    public:    
    ImageProcessor();
    void PrepImage(ImageHolder<uint8_t> &image);
    void normaliseImageIntensity(ImageHolder<uint8_t> &img);
    void scaleIntensity(int factor, ImageHolder<uint8_t> &image) const;

    Point GetCoordinateOfHighestValueDirection(const Point &p, const ImageHolder<uint8_t> &image) const;
    std::vector<std::vector<uint8_t>> getNeighbourhood(const Point &p, const ImageHolder<uint8_t> &image) const;


private:

    ImageHolder<uint8_t> ConvolveImage(const std::vector<std::vector<float>>& kernel, const ImageHolder<uint8_t> &image, bool OnlyAbsoluteValues);

    ImageHolder<uint8_t> ComputeGradientMagnitude(const ImageHolder<uint8_t> &image);

    void BlurImage(int blurAmount, ImageHolder<uint8_t> &image);
};

#endif // CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGE_HPP