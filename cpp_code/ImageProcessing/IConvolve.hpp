#ifndef ICONVOLVE_HPP
#define ICONVOLVE_HPP

#include "DataObjects/ImageHolder.hpp"
#include "DataObjects/Point.hpp"


class IConvolver {
    public:
    virtual ~IConvolver() = default;
    virtual ImageHolder<float> Convolve(const std::vector<std::vector<float>>& kernel,
                                        const ImageHolder<float>& image) const = 0;
};

#endif // ICONVOLVE_HPP