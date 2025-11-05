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

    int getWidth() const;
    int getHeight() const;
    Point getCenter() const;

    T getPixel(const Point) const;
    T getPixel(int x, int y) const;

    // overload [] operator to access pixel values
    T operator[](const Point& p) const;
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