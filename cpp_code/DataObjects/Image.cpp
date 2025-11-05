#include "Image.hpp"
#include "Point.hpp"


Image::Image(const uint8_t *data, int width, int height, int stride)
    : m_width(width), m_height(height), m_stride(stride)
{
    ReconstructImage(data);
}

void Image::PrepImage()
{
    BlurImage(8);
    ComputeGradientMagnitude();
    BlurImage(3);
    normaliseImageIntensity();
}

void Image::normaliseImageIntensity()
{
    // multiple with the maximum value to scale to 255
    uint8_t maxVal = 0;
    for (const auto& row : m_image)
    {
        for (uint8_t v : row)
        {
            if (v > maxVal) maxVal = v;
        }
    }
    if (maxVal == 0) return; // avoid division by zero
    double scale = 255.0 / static_cast<double>(maxVal);
    for (auto& row : m_image)
    {
        for (auto& v : row)
        {
            v = static_cast<uint8_t>(std::min(255.0, v * scale));
        }
    }
}

int Image::GetWidth() const
{
    return m_width;
}

int Image::GetHeight() const
{
    return m_height;
}

const std::vector<std::vector<uint8_t>>& Image::GetImageVector() const
{
    return m_image;
}

Point Image::GetCenter() const
{
    return Point(m_width / 2, m_height / 2);
}

Point Image::GetCoordinateOfHighestValueDirection(const Point &p) const
{
    // get the highest gradient direction around point p (8 connectivity)
    uint8_t maxGradient = 0;
    if (p.X >= 0 && p.X < m_width && p.Y >= 0 && p.Y < m_height)
    {
        maxGradient = m_image[p.Y][p.X];
    }
    Point bestPoint = p;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (dx == 0 && dy == 0) continue; // skip the center point
            int nx = p.X + dx;
            int ny = p.Y + dy;
            if (nx >= 0 && nx < m_width && ny >= 0 && ny < m_height)
            {
                uint8_t gradientValue = m_image[ny][nx];
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

std::vector<std::vector<uint8_t>> Image::getNeighbourhood(const Point &p) const
{
    std::vector<std::vector<uint8_t>> neighbourhood;
    
    neighbourhood.resize(3, std::vector<uint8_t>(3, 0));
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            int nx = p.X + dx;
            int ny = p.Y + dy;
            if (nx >= 0 && nx < m_width && ny >= 0 && ny < m_height)
            {
                neighbourhood[dy + 1][dx + 1] = m_image[ny][nx];
            }
        }
    }
    return neighbourhood;
}

// convert the image back to vector of vectors
void Image::ReconstructImage(const uint8_t *data)
{
    m_image.resize(m_height, std::vector<uint8_t>(m_width));
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            m_image[y][x] = data[y * m_stride + x];
        }
    }
}

void Image::ConvolveImage(const std::vector<std::vector<float>>& kernel, bool OnlyAbsoluteValues)
{
    int kHeight = static_cast<int>(kernel.size());
    int kWidth = static_cast<int>(kernel[0].size());
    int kHalfHeight = kHeight / 2;
    int kHalfWidth = kWidth / 2;

    std::vector<std::vector<uint8_t>> convolvedImage(m_height, std::vector<uint8_t>(m_width, 0));

    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            float sum = 0.0f;
            for (int ky = -kHalfHeight; ky <= kHalfHeight; ++ky)
            {
                for (int kx = -kHalfWidth; kx <= kHalfWidth; ++kx)
                {
                    int ny = y + ky;
                    int nx = x + kx;
                    if (ny >= 0 && ny < m_height && nx >= 0 && nx < m_width)
                    {
                        sum += kernel[ky + kHalfHeight][kx + kHalfWidth] * m_image[ny][nx];
                    }
                }
            }

            sum = (OnlyAbsoluteValues) ? std::abs(sum) : sum;
            convolvedImage[y][x] = static_cast<uint8_t>(std::clamp(static_cast<int>(sum), 0, 255));
        }
    }

    m_image = std::move(convolvedImage);
}


void Image::ComputeGradientMagnitude()
{
    // compute signed Sobel gradients directly into int buffers (preserve sign)
    static const int kx[3][3] = {
        { -1, 0, 1 },
        { -2, 0, 2 },
        { -1, 0, 1 }
    };
    static const int ky[3][3] = {
        {  1,  2,  1 },
        {  0,  0,  0 },
        { -1, -2, -1 }
    };

    std::vector<std::vector<int>> gradX(m_height, std::vector<int>(m_width, 0));
    std::vector<std::vector<int>> gradY(m_height, std::vector<int>(m_width, 0));

    // compute signed gradients (avoid border pixels)
    for (int y = 1; y < m_height - 1; ++y)
    {
        for (int x = 1; x < m_width - 1; ++x)
        {
            int sx = 0;
            int sy = 0;
            for (int ky_i = -1; ky_i <= 1; ++ky_i)
            {
                for (int kx_i = -1; kx_i <= 1; ++kx_i)
                {
                    int px = x + kx_i;
                    int py = y + ky_i;
                    int p = static_cast<int>(m_image[py][px]);
                    sx += kx[ky_i + 1][kx_i + 1] * p;
                    sy += ky[ky_i + 1][kx_i + 1] * p;
                }
            }
            gradX[y][x] = sx;
            gradY[y][x] = sy;
        }
    }

    // combine signed gradients into magnitude and store as uint8_t
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            int gx = gradX[y][x];
            int gy = gradY[y][x];
            int magnitude = static_cast<int>(std::sqrt(static_cast<double>(gx * gx + gy * gy)));
            int clamped = std::clamp(magnitude, 0, 255);
            m_image[y][x] = static_cast<uint8_t>(255 - clamped);
        }
    }
}

void Image::BlurImage(int blurAmount)
{
    // Take vector of vectors m_image and apply a simple box blur with the given blurAmount
    std::vector<std::vector<uint8_t>> blurredImage = m_image;
    int kernelSize = 2 * blurAmount + 1;
    int halfKernel = blurAmount;

    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            int sum = 0;
            int count = 0;

            for (int ky = -halfKernel; ky <= halfKernel; ++ky)
            {
                for (int kx = -halfKernel; kx <= halfKernel; ++kx)
                {
                    int ny = y + ky;
                    int nx = x + kx;
                    if (ny >= 0 && ny < m_height && nx >= 0 && nx < m_width)
                    {
                        sum += m_image[ny][nx];
                        count++;
                    }
                }
            }

            blurredImage[y][x] = static_cast<uint8_t>(sum / count);
        }
    }

    m_image = std::move(blurredImage);
}

std::vector<std::vector<uint8_t>> Image::scaleIntensity(int factor) const
{
    std::vector<std::vector<uint8_t>> scaledImage = m_image;
    for (auto& row : scaledImage)
    {
        for (auto& pixel : row)
        {
            pixel = static_cast<uint8_t>(std::clamp(static_cast<int>(pixel) / factor, 0, 255));
        }
    }
    return scaledImage;
}