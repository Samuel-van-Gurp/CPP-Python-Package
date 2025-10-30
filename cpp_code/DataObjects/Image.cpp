#include "Image.hpp"
#include "Point.hpp"

Image::Image(const uint8_t *data, int width, int height, int stride)
    : m_width(width), m_height(height), m_stride(stride)
{
    ReconstructImage(data);
    BlurImage(8);
    ConstructImageGradient();
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

void Image::ConstructImageGradient()
{
    if (m_height <= 0 || m_width <= 0) return;
    if (m_height < 3 || m_width < 3) {
        // too small to compute central-difference gradient, leave as-is or zero
        m_image.assign(m_height, std::vector<uint8_t>(m_width, 0));
        return;
    }

    // Use a separate buffer so we don't overwrite source pixels while computing
    const auto src = m_image; // copy of current image pixels
    std::vector<std::vector<uint8_t>> grad(m_height, std::vector<uint8_t>(m_width, 0));

    for (int y = 1; y < m_height - 1; ++y)
    {
        for (int x = 1; x < m_width - 1; ++x)
        {
            int gx = static_cast<int>(src[y][x + 1]) - static_cast<int>(src[y][x - 1]);
            int gy = static_cast<int>(src[y + 1][x]) - static_cast<int>(src[y - 1][x]);
            int magnitude = static_cast<int>(std::sqrt(static_cast<double>(gx * gx + gy * gy)));
            if (magnitude > 255) magnitude = 255;
            grad[y][x] = static_cast<uint8_t>(magnitude);
        }
    }

    m_image = std::move(grad);
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