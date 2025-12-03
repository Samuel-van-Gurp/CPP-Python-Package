#include "ImageProcessorFacade.hpp"


ImageProcessorFacade::ImageProcessorFacade(std::unique_ptr<IConvolver> convolver, std::unique_ptr<IIntensityManipulator> intensityManipulator)
    : m_convolver(std::move(convolver))
    , m_intensityManipulator(std::move(intensityManipulator))
{    
}

void ImageProcessorFacade::PrepImage(ImageHolder<float>& image)
{
    // BlurImage(BlurType::Large, image);
    auto gradientMagnitude = ComputeGradientMagnitude(image);
    BlurImage(BlurType::Large, gradientMagnitude);
    m_intensityManipulator->normaliseImageIntensity(gradientMagnitude);
    m_intensityManipulator->invertImageIntensity(gradientMagnitude);

    image = std::move(gradientMagnitude);
}

void ImageProcessorFacade::normaliseImageIntensity(ImageHolder<float>& img)
{
    m_intensityManipulator->normaliseImageIntensity(img);
}

void ImageProcessorFacade::scaleIntensity(int factor, ImageHolder<float> &image) const
{
    m_intensityManipulator->scaleIntensity(factor, image);
}

void ImageProcessorFacade::invertImageIntensity(ImageHolder<float> &image) const
{
    m_intensityManipulator->invertImageIntensity(image);
}

ImageHolder<float> ImageProcessorFacade::ComputeGradientMagnitude(const ImageHolder<float> &image)
{
    // compute signed Sobel gradients directly into vector buffers (preserve sign)
    static const std::vector<std::vector<float>> kx = {
        { -1, 0, 1 },
        { -2, 0, 2 },
        { -1, 0, 1 }
    };
    static const std::vector<std::vector<float>> ky = {
        {  1,  2,  1 },
        {  0,  0,  0 },
        { -1, -2, -1 }
    };

    int w = image.getWidth();
    int h = image.getHeight();

    ImageHolder<float> gradMagHolder(std::vector<float>(w * h, 0), w, h);

    ImageHolder<float> gradXHolder = m_convolver->Convolve(kx, image); 
    ImageHolder<float> gradYHolder = m_convolver->Convolve(ky, image); 

    // combine signed gradients into magnitude
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            float gx = gradXHolder.getPixel(x, y);
            float gy = gradYHolder.getPixel(x, y);
            gradMagHolder.setPixel(x, y, std::hypot(gx, gy));
        }
    }

    return gradMagHolder;   
}

void ImageProcessorFacade::BlurImage(BlurType blurType, ImageHolder<float> &image)
{
    // Normalised gaussian kernels
    static const std::vector<std::vector<float>> smallBlurKernel = {
        { 1/16.0f, 2/16.0f, 1/16.0f },
        { 2/16.0f, 4/16.0f, 2/16.0f },
        { 1/16.0f, 2/16.0f, 1/16.0f }
    };
    static const std::vector<std::vector<float>> mediumBlurKernel = {
        { 1/273.0f, 4/273.0f, 7/273.0f, 4/273.0f, 1/273.0f },
        { 4/273.0f,16/273.0f,26/273.0f,16/273.0f, 4/273.0f },
        { 7/273.0f,26/273.0f,41/273.0f,26/273.0f, 7/273.0f },
        { 4/273.0f,16/273.0f,26/273.0f,16/273.0f, 4/273.0f },
        { 1/273.0f, 4/273.0f, 7/273.0f, 4/273.0f, 1/273.0f }
    };

    static const std::vector<std::vector<float>> largeBlurKernel = {
        { 1/1003.0f,  8/1003.0f, 28/1003.0f, 56/1003.0f, 70/1003.0f, 56/1003.0f, 28/1003.0f,  8/1003.0f, 1/1003.0f },
        { 8/1003.0f, 64/1003.0f,224/1003.0f,448/1003.0f,560/1003.0f,448/1003.0f,224/1003.0f, 64/1003.0f, 8/1003.0f },
        {28/1003.0f,224/1003.0f,784/1003.0f,1568/1003.0f,1960/1003.0f,1568/1003.0f,784/1003.0f,224/1003.0f,28/1003.0f },
        {56/1003.0f,448/1003.0f,1568/1003.0f,3136/1003.0f,3920/1003.0f,3136/1003.0f,1568/1003.0f,448/1003.0f,56/1003.0f },
        {70/1003.0f,560/1003.0f,1960/1003.0f,3920/1003.0f,4900/1003.0f,3920/1003.0f,1960/1003.0f,560/1003.0f,70/1003.0f },
        {56/1003.0f,448/1003.0f,1568/1003.0f,3136/1003.0f,3920/1003.0f,3136/1003.0f,1568/1003.0f,448/1003.0f,56/1003.0f },
        {28/1003.0f,224/1003.0f,784/1003.0f,1568/1003.0f,1960/1003.0f,1568/1003.0f,784/1003.0f,224/1003.0f,28/1003.0f },
        { 8/1003.0f, 64/1003.0f,224/1003.0f,448/1003.0f,560/1003.0f,448/1003.0f,224/1003.0f, 64/1003.0f, 8/1003.0f },
        { 1/1003.0f,  8/1003.0f, 28/1003.0f, 56/1003.0f, 70/1003.0f, 56/1003.0f, 28/1003.0f,  8/1003.0f, 1/1003.0f }
    };

    switch (blurType)
    {
    case BlurType::Small:
        image = m_convolver->Convolve(smallBlurKernel, image);
        break;
    case BlurType::Medium:
        image = m_convolver->Convolve(mediumBlurKernel, image);
        break;
    case BlurType::Large:
        image = m_convolver->Convolve(largeBlurKernel, image);
        break;
    default:
        image = m_convolver->Convolve(mediumBlurKernel, image);
        break;
    }
}