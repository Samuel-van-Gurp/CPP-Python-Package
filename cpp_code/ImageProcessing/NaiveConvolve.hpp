#include "IConvolve.hpp"

class NaiveConvolve : public IConvolver
{   
public:
    NaiveConvolve();
    ImageHolder<float> Convolve(const std::vector<std::vector<float>>& kernel,
                                        const ImageHolder<float>& image) const override;

};
