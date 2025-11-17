#include "IntensityManipulator.hpp"

IntensityManipulator::IntensityManipulator()
{
}
void IntensityManipulator::scaleIntensity(int factor, ImageHolder<float> &image) const
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

void IntensityManipulator::normaliseImageIntensity(ImageHolder<float>& img) const
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

void IntensityManipulator::invertImageIntensity(ImageHolder<float> &image) const
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