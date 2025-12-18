#ifndef IMAGE_HOLDER_2CHANNEL_HPP
#define IMAGE_HOLDER_2CHANNEL_HPP

#include <exception>
#include <iostream>
#include "DataObjects/ImageHolderBase.hpp"
#include "DataObjects/ImageHolder.hpp" 

template <typename T>
class ImageHolder2channel : public ImageHolderBase<T>
{

public:
    // init from pointer to data
    ImageHolder2channel(ImageHolder<T> &imageHolderChannel1, ImageHolder<T> &imageHolderChannel2);

    int getWidth() const;
    int getHeight() const;
    Point getCenter() const;
    const std::vector<T>& getDataVector(int channel) const;
    void setPixel(int x, int y, int channel, T value);
    T getPixel(int x, int y, int channel) const;
    T getPixel(const Point p, int channel) const;

    // bool empty() const;
private:
    void checkImageDimensions(const ImageHolder<T> &imageHolderChannel1, const ImageHolder<T> &imageHolderChannel2) const;
    std::vector<T> m_data_channel1;
    std::vector<T> m_data_channel2;
};

template <typename T>
int ImageHolder2channel<T>::getWidth() const
{
    return this->m_width;
}

template <typename T>
int ImageHolder2channel<T>::getHeight() const
{
    return this->m_height;
}

template <typename T>
Point ImageHolder2channel<T>::getCenter() const
{
    return Point(this->m_width / 2, this->m_height / 2);
}

template <typename T>
const std::vector<T>& ImageHolder2channel<T>::getDataVector(int channel) const
{
    if (channel == 1) {
        return m_data_channel1;
    } else if (channel == 2) {
        return m_data_channel2;
    } else {
        throw std::out_of_range("Invalid channel number");
    }
}

template <typename T>
ImageHolder2channel<T>::ImageHolder2channel(ImageHolder<T> &imageHolderChannel1, ImageHolder<T> &imageHolderChannel2)
: m_data_channel1(imageHolderChannel1.getDataVector()),
  m_data_channel2(imageHolderChannel2.getDataVector())
{


    this->m_width = imageHolderChannel1.getWidth();
    this->m_height = imageHolderChannel1.getHeight();
    this->m_stride = this->m_width; // todo: check if this is still used alo in the base class

}

template <typename T>
void ImageHolder2channel<T>::checkImageDimensions(const ImageHolder<T> &imageHolderChannel1, const ImageHolder<T> &imageHolderChannel2) const
{
    try {
        if ( (imageHolderChannel1.getWidth() != imageHolderChannel2.getWidth()) ||
             (imageHolderChannel1.getHeight() != imageHolderChannel2.getHeight()) ) 
        {
            throw std::invalid_argument("ImageHolder2channel: Channel images must have the same dimensions.");
        }
    } 
    catch (const std::invalid_argument& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }

}

#endif //IMAGE_HOLDER_2CHANNEL_HPP 