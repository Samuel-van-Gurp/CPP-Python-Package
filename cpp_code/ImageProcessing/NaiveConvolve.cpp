#include "NaiveConvolve.hpp"

NaiveConvolve::NaiveConvolve() = default;

ImageHolder<float> NaiveConvolve::Convolve(const std::vector<std::vector<float>>& kernel,
                                        const ImageHolder<float>& image) const
{
    if (kernel.empty() || kernel[0].empty()) 
    {
        return image;
    }
    int kHeight = static_cast<int>(kernel.size());
    int kWidth = static_cast<int>(kernel[0].size());
    int kHalfHeight = kHeight / 2;
    int kHalfWidth = kWidth / 2;

    std::vector<float> convolvedImage(image.getHeight() * image.getWidth(), 0);
    ImageHolder<float> convolvedHolder(std::move(convolvedImage), image.getWidth(), image.getHeight());

    for (int y = 0; y < image.getHeight(); ++y)
    {
        for (int x = 0; x < image.getWidth(); ++x)
        {
            float sum = 0.0f;
            for (int ky = -kHalfHeight; ky <= kHalfHeight; ++ky)
            {
                for (int kx = -kHalfWidth; kx <= kHalfWidth; ++kx)
                {
                    int ny = y + ky;
                    int nx = x + kx;
                    if (ny >= 0 && ny < image.getHeight() && nx >= 0 && nx < image.getWidth())
                    {
                        sum += kernel[ky + kHalfHeight][kx + kHalfWidth] * image.getPixel(nx, ny);
                    }
                }
            }
            convolvedHolder.setPixel(x, y, sum);
        }
    }
    return convolvedHolder;
}