#ifndef CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGEHOLDER_HPP
#define CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGEHOLDER_HPP

#include <cstdint>
#include <vector>
#include <cmath>
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

    std::vector<std::vector<T>> getNeighbourhood(const Point &p) const;


    template<typename U>
    static ImageHolder<T> StaticFactoryTypeChanger(const U* data, int width, int height, int stride);

    int getWidth() const;
    int getHeight() const;
    Point getCenter() const;
    const std::vector<T>& getDataVector() const;
    T getMaxIntensityInImage() const;
    T getMinIntesityInImage() const;
    void setPixel(int x, int y, T value);

    T getPixel(const Point) const;
    T getPixel(float x, float y) const;

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

template <typename T>
inline std::vector<std::vector<T>> ImageHolder<T>::getNeighbourhood(const Point &p) const
{
    std::vector<std::vector<float>> neighbourhood(3, std::vector<float>(3, 0.0f));
    
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            int nx = p.X + dx;
            int ny = p.Y + dy;
            if (nx >= 0 && nx < m_width && ny >= 0 && ny < m_height)
            {
                neighbourhood[dy + 1][dx + 1] = getPixel(nx, ny);
            }
        }
    }
    return neighbourhood;
}

template<typename T>
template<typename U>
ImageHolder<T> ImageHolder<T>::StaticFactoryTypeChanger(const U* data, int width, int height, int stride)
{
    if (data == nullptr || width <= 0 || height <= 0 || stride < width)
    {
        return ImageHolder<T>(std::vector<T>(), 0, 0);
    }

    std::vector<T> buffer(static_cast<size_t>(width) * static_cast<size_t>(height));

    for (int y = 0; y < height; ++y)
    {
        const U* row = data + static_cast<size_t>(y) * static_cast<size_t>(stride);
        for (int x = 0; x < width; ++x)
        {
            buffer[static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x)] = static_cast<T>(row[x]);
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
const std::vector<T>& ImageHolder<T>::getDataVector() const
{
    return m_data;
}

template <typename T>
inline T ImageHolder<T>::getMaxIntensityInImage() const
{
    if (m_data.empty())
        return T();
    T maxIntensity = m_data[0];
    for (const T& value : m_data)
    {
        if (value > maxIntensity)
        {
            maxIntensity = value;
        }
    }
    return maxIntensity;
}

template <typename T>
inline T ImageHolder<T>::getMinIntesityInImage() const
{
    if (m_data.empty())
        return T();
    T minIntensity = m_data[0];
    for (const T& value : m_data)
    {
        if (value < minIntensity)
        {
            minIntensity = value;
        }
    }
    return minIntensity;
}

template<typename T>
void ImageHolder<T>::setPixel(int x, int y, T value)
{
    // bounds check
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return;
    }
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
    return getPixel(p.X, p.Y);
}

template<typename T>
T ImageHolder<T>::getPixel(float x, float y) const
{
    // bounds check
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return T(0); 
    }
    // TODO: use interpolation instead of rounding
    // round to nearest integer pixel coordinates
    return m_data[static_cast<int>(round(y)) * m_stride + static_cast<int>(round(x))];
}

#endif // CPP_PY_PACKAGE_CPP_CODE_DATAOBJECTS_IMAGEHOLDER_HPP