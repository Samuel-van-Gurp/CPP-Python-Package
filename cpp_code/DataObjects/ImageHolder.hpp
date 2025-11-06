#ifndef CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGEHOLDER_HPP
#define CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGEHOLDER_HPP

#include <cstdint>
#include <vector>
#include "Point.hpp"

template <typename T>
class ImageHolder
{
private:
    int m_width;
    int m_height;
    int m_stride;
    std::vector<T> m_data;
public:
    // init from pointer to data
    ImageHolder(const T *data, int width, int height, int stride);

    // init from vector
    ImageHolder(std::vector<T> data, int width, int height);

    template<typename U>
    static ImageHolder<T> StaticFactoryTypeChanger(const U* data, int width, int height, int stride);

    int getWidth() const;
    int getHeight() const;
    Point getCenter() const;
    std::vector<T>& getDataVector();
    void setPixel(int x, int y, T value);

    T getPixel(const Point) const;
    T getPixel(int x, int y) const;

    // overload [] operator to access pixel values
    T operator[](const Point& p) const;

    bool empty() const;
};


template<typename T>
ImageHolder<T>::ImageHolder(const T *data, int width, int height, int stride)
    : m_data(data, data + height * stride), m_width(width), m_height(height), m_stride(stride)
{
}

template<typename T>
ImageHolder<T>::ImageHolder(std::vector<T> data, int width, int height)
    : m_data(std::move(data)), m_width(width), m_height(height), m_stride(width)
{
}

// use this static factory method to create ImageHolder from pointer to different data type
template<typename T>
template<typename U>
ImageHolder<T> ImageHolder<T>::StaticFactoryTypeChanger(const U* data, int width, int height, int stride)
{
    std::vector<T> buffer;
    buffer.reserve(static_cast<size_t>(height) * stride);
    for (int y = 0; y < height; ++y)
    {
        const U* row = data + static_cast<size_t>(y) * stride;
        for (int x = 0; x < stride; ++x)
        {
            buffer.push_back(static_cast<T>(row[x]));
        }
    }
    return ImageHolder<T>(std::move(buffer), width, height);
}

// getters for image properties
template<typename T>
int ImageHolder<T>::getWidth() const
{
    return m_width;
}
template<typename T>
int ImageHolder<T>::getHeight() const
{
    return m_height;
}

template<typename T>
Point ImageHolder<T>::getCenter() const
{
    return Point(m_width / 2, m_height / 2);
}

template<typename T>
std::vector<T>& ImageHolder<T>::getDataVector()
{
    return m_data;
}

template<typename T>
void ImageHolder<T>::setPixel(int x, int y, T value)
{
    m_data[y * m_stride + x] = value;
}
template<typename T>
bool ImageHolder<T>::empty() const
{
    return m_data.empty();
}

template<typename T>
T ImageHolder<T>::getPixel(const Point p) const
{
    return m_data[p.Y * m_stride + p.X];
}

template<typename T>
T ImageHolder<T>::getPixel(int x, int y) const
{
    return m_data[y * m_stride + x];
}

template<typename T>
T ImageHolder<T>::operator[](const Point& p) const
{
    return getPixel(p);
}


#endif // CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGEHOLDER_HPP