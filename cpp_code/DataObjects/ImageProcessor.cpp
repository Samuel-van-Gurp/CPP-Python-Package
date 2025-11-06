#include "ImageProcessor.hpp"


ImageProcessor::ImageProcessor()
{    
}

void ImageProcessor::PrepImage(ImageHolder<uint8_t>& image)
{
    BlurImage(1, image);
    auto gradientMagnitude = ComputeGradientMagnitude(image);
    BlurImage(1, gradientMagnitude);
    normaliseImageIntensity(gradientMagnitude);

    image = std::move(gradientMagnitude);
}

void ImageProcessor::normaliseImageIntensity(ImageHolder<uint8_t>& img)
{
    // multiple with the maximum value to scale to 255
    uint8_t maxVal = 0;
    for (int i = 0; i < img.getHeight(); ++i)
    {
        for (int j = 0; j < img.getWidth(); ++j)
        {
            uint8_t v = img.getPixel(j, i);
            if (v > maxVal) maxVal = v;
        }
    }

    if (maxVal == 0) return; // avoid division by zero
    double scale = 255.0 / static_cast<double>(maxVal);
    for (int i = 0; i < img.getHeight(); ++i)
    {
        for (int j = 0; j < img.getWidth(); ++j)
        {
            uint8_t v = img.getPixel(j, i);
            v = static_cast<uint8_t>(std::min(255.0, v * scale));
            img.setPixel(j, i, v);
        }
    }
}

Point ImageProcessor::GetCoordinateOfHighestValueDirection(const Point &p, const ImageHolder<uint8_t>& image) const
{
    // get the highest gradient direction around point p (8 connectivity)
    uint8_t maxGradient = 0;
    if (p.X >= 0 && p.X < image.getWidth() && p.Y >= 0 && p.Y < image.getHeight())
    {
        maxGradient = image.getPixel(p.X, p.Y);
    }
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
                uint8_t gradientValue = image.getPixel(nx, ny);
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

std::vector<std::vector<uint8_t>> ImageProcessor::getNeighbourhood(const Point &p, const ImageHolder<uint8_t> &image) const
{
    std::vector<std::vector<uint8_t>> neighbourhood;
    
    neighbourhood.resize(3, std::vector<uint8_t>(3, 0));
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            int nx = p.X + dx;
            int ny = p.Y + dy;
            if (nx >= 0 && nx < image.getWidth() && ny >= 0 && ny < image.getHeight())
            {
                neighbourhood[dy + 1][dx + 1] = image.getPixel(nx, ny);
            }
        }
    }
    return neighbourhood;
}


ImageHolder<uint8_t> ImageProcessor::ConvolveImage(const std::vector<std::vector<float>>& kernel, const ImageHolder<uint8_t> &image, bool OnlyAbsoluteValues)
{
    int kHeight = static_cast<int>(kernel.size());
    int kWidth = static_cast<int>(kernel[0].size());
    int kHalfHeight = kHeight / 2;
    int kHalfWidth = kWidth / 2;

    std::vector<uint8_t> convolvedImage(image.getHeight() * image.getWidth(), 0);
    ImageHolder<uint8_t> convolvedHolder(std::move(convolvedImage), image.getWidth(), image.getHeight());

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

            sum = (OnlyAbsoluteValues) ? std::abs(sum) : sum;
            convolvedHolder.setPixel(x, y, static_cast<uint8_t>(std::clamp(static_cast<int>(sum), 0, 255)));
        }
    }
    return convolvedHolder;
}


ImageHolder<uint8_t> ImageProcessor::ComputeGradientMagnitude(const ImageHolder<uint8_t> &image)
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

    ImageHolder<uint8_t> gradMagHolder(std::vector<uint8_t>(image.getWidth() * image.getHeight(), 0), image.getWidth(), image.getHeight());

    ImageHolder<uint8_t> gradXHolder = ConvolveImage(kx, image, true);
    ImageHolder<uint8_t> gradYHolder = ConvolveImage(ky, image, true);

    // combine signed gradients into magnitude and store as uint8_t
    for (int y = 0; y < image.getHeight(); ++y)
    {
        for (int x = 0; x < image.getWidth(); ++x)
        {
            int gx = gradXHolder.getPixel(x, y);
            int gy = gradYHolder.getPixel(x, y);
            int magnitude = static_cast<int>(std::sqrt(static_cast<double>(gx * gx + gy * gy)));
            int clamped = std::clamp(magnitude, 0, 255);
            gradMagHolder.setPixel(x, y, static_cast<uint8_t>(255 - clamped));
        }
    }

    return gradMagHolder;   
}

void ImageProcessor::BlurImage(int blurAmount, ImageHolder<uint8_t> &image)
{
    // Normalised gaussian kernel
    std::vector<std::vector<float>> smallBlurKernel = {
        { 1/16.0f, 2/16.0f, 1/16.0f },
        { 2/16.0f, 4/16.0f, 2/16.0f },
        { 1/16.0f, 2/16.0f, 1/16.0f }
    };
    std::vector<std::vector<float>> mediumBlurKernel = {
        { 1/273.0f, 4/273.0f, 7/273.0f, 4/273.0f, 1/273.0f },
        { 4/273.0f,16/273.0f,26/273.0f,16/273.0f, 4/273.0f },
        { 7/273.0f,26/273.0f,41/273.0f,26/273.0f, 7/273.0f },
        { 4/273.0f,16/273.0f,26/273.0f,16/273.0f, 4/273.0f },
        { 1/273.0f, 4/273.0f, 7/273.0f, 4/273.0f, 1/273.0f }
    };
    image = ConvolveImage(mediumBlurKernel, image, false);
}


void ImageProcessor::scaleIntensity(int factor, ImageHolder<uint8_t> &image) const
{
    ImageHolder<uint8_t> scaledImage = image; // make a copy to store scaled values


    for (int i = 0; i < image.getHeight(); ++i)
    {
        for (int j = 0; j < image.getWidth(); ++j)
        {
            uint8_t v = image.getPixel(j, i);
            v = static_cast<uint8_t>(std::clamp(static_cast<int>(v) / factor, 0, 255));
            scaledImage.setPixel(j, i, v);
        }
    }
    image = std::move(scaledImage);
}