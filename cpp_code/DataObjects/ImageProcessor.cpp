#include "ImageProcessor.hpp"


ImageProcessor::ImageProcessor()
{    
}

void ImageProcessor::PrepImage(ImageHolder<float>& image)
{
    BlurImage(BlurType::Medium, image);
    auto gradientMagnitude = ComputeGradientMagnitude(image);
    BlurImage(BlurType::Medium, gradientMagnitude);
    normaliseImageIntensity(gradientMagnitude);
    invertImageIntensity(gradientMagnitude);

    image = std::move(gradientMagnitude);
}

void ImageProcessor::normaliseImageIntensity(ImageHolder<float>& img)
{
    int w = img.getWidth();
    int h = img.getHeight();

    float maxVal = img.getMaxIntensityInImage();

    if (maxVal == 0) return; // avoid division by zero
    float scale = 1.0f / static_cast<float>(maxVal);
    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            float v = img.getPixel(j, i);
            v = static_cast<float>(std::min(1.0f, v * scale));
            img.setPixel(j, i, v);
        }
    }
}

Point ImageProcessor::GetCoordinateOfMaximumNeighborValue(const Point &p, const ImageHolder<float>& image) const
{
    // get the highest value direction around point p (8 connectivity)
    float maxGradient = -std::numeric_limits<float>::infinity();

    Point bestPoint = p;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (dx == 0 && dy == 0) continue; // skip the center point
            int nx = p.X + dx;
            int ny = p.Y + dy;
            if (nx >= 0 && nx < image.getWidth() && ny >= 0 && ny < image.getHeight())
            {
                float gradientValue = image.getPixel(nx, ny);
                if (gradientValue > maxGradient)
                {
                    maxGradient = gradientValue;
                    bestPoint = Point(nx, ny);
                }
            }
        }
    }
    return bestPoint;
}

std::vector<std::vector<float>> ImageProcessor::getNeighbourhood(const Point &p, const ImageHolder<float> &image) const
{
    int w = image.getWidth();
    int h = image.getHeight();
    std::vector<std::vector<float>> neighbourhood(3, std::vector<float>(3, 0.0f));
    
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            int nx = p.X + dx;
            int ny = p.Y + dy;
            if (nx >= 0 && nx < w && ny >= 0 && ny < h)
            {
                neighbourhood[dy + 1][dx + 1] = image.getPixel(nx, ny);
            }
        }
    }
    return neighbourhood;
}


ImageHolder<float> ImageProcessor::ConvolveImage(const std::vector<std::vector<float>>& kernel, const ImageHolder<float> &image) const
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


ImageHolder<float> ImageProcessor::ComputeGradientMagnitude(const ImageHolder<float> &image)
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

    ImageHolder<float> gradXHolder = ConvolveImage(kx, image);
    ImageHolder<float> gradYHolder = ConvolveImage(ky, image);

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

void ImageProcessor::BlurImage(BlurType blurType, ImageHolder<float> &image)
{
    // Normalised gaussian kernel
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
        image = ConvolveImage(smallBlurKernel, image);
        break;
    case BlurType::Medium:
        image = ConvolveImage(mediumBlurKernel, image);
        break;
    case BlurType::Large:
        image = ConvolveImage(largeBlurKernel, image);
        break;
    default:
        image = ConvolveImage(mediumBlurKernel, image);
        break;
    }
}


void ImageProcessor::scaleIntensity(int factor, ImageHolder<float> &image) const
{
    if (factor == 0) return; // avoid division by zero
    
    int w = image.getWidth();
    int h = image.getHeight();

    ImageHolder<float> scaledImage = image; // make a copy to store scaled values

    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            float v = image.getPixel(j, i);
            float vs = v / static_cast<float>(factor);
            scaledImage.setPixel(j, i, vs);
        }
    }
    image = std::move(scaledImage);
}

void ImageProcessor::invertImageIntensity(ImageHolder<float> &image) const
{
    int w = image.getWidth();
    int h = image.getHeight();

    // assuming image intensity in range of [0,1]
    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            float v = image.getPixel(j, i);
            v = 1.0f - v;
            image.setPixel(j, i, v);
        }
    }
}