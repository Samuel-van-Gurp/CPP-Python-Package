#include "Image.hpp"

Image::Image(const uint8_t *data, int width, int height, int stride)
    : m_data(data), m_width(width), m_height(height), m_stride(stride)
{
    ReconstructImage();
    BlurImage(5);
    ConstructImageGradient();
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

// convert the image back to vector of vectors
void Image::ReconstructImage()
{
    m_image.resize(m_height, std::vector<uint8_t>(m_width));
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            m_image[y][x] = m_data[y * m_stride + x];
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

// function save intermediate image as PGM file, for debugging purpose
void Image::SaveAsPGM()
{
    // Validate input
    if (!m_data) {
        std::cerr << "SaveAsPGM error: m_data is null\n";
        return;
    }
    if (m_width <= 0 || m_height <= 0) {
        std::cerr << "SaveAsPGM error: invalid dimensions (w=" << m_width
                  << " h=" << m_height << ")\n";
        return;
    }
    if (static_cast<int>(m_image.size()) != m_height) {
        std::cerr << "SaveAsPGM error: m_image height mismatch (" << m_image.size() << " != " << m_height << ")\n";
        return;
    }

    std::string filename = "C:\\Users\\svangurp\\Desktop\\CPP_PY_package\\DebugImage\\gradient.pgm";

    // Build a single contiguous buffer from m_image (removes any row-fragmentation issues)
    std::vector<uint8_t> buffer;
    buffer.reserve(static_cast<size_t>(m_width) * static_cast<size_t>(m_height));
    for (int y = 0; y < m_height; ++y) {
        if (static_cast<int>(m_image[y].size()) < m_width) {
            std::cerr << "SaveAsPGM error: m_image row " << y << " too small (" << m_image[y].size() << ")\n";
            return;
        }
        buffer.insert(buffer.end(), m_image[y].begin(), m_image[y].begin() + m_width);
    }

    // debug: range check
    uint8_t minv = 255, maxv = 0;
    for (uint8_t v : buffer) { if (v < minv) minv = v; if (v > maxv) maxv = v; }
    std::cout << "SaveAsPGM: writing buffer (w=" << m_width << " h=" << m_height << ") min=" << int(minv) << " max=" << int(maxv) << "\n";

    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "SaveAsPGM error: failed to open '" << filename << "' for writing\n";
        return;
    }

    out << "P5\n" << m_width << " " << m_height << "\n255\n";
    out.write(reinterpret_cast<const char*>(buffer.data()), static_cast<std::streamsize>(buffer.size()));
    out.close();

    if (!out) {
        std::cerr << "SaveAsPGM error: write failed\n";
        return;
    }

    // validate file size matches expected bytes (header + data)
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "SaveAsPGM error: cannot re-open '" << filename << "' for validation\n";
        return;
    }

    // read header (skip comments), then compute remaining bytes
    std::string magic;
    in >> magic;
    if (magic != "P5") {
        std::cerr << "SaveAsPGM validation: unexpected magic '" << magic << "'\n";
        in.close();
        return;
    }
    int w = 0, h = 0, maxv_read = 0;
    in >> w >> h >> maxv_read;
    // consume single whitespace char after header (per PGM spec)
    in.get();

    std::streampos pos = in.tellg();
    in.seekg(0, std::ios::end);
    std::streamoff fileSize = in.tellg();
    std::streamoff dataBytes = fileSize - pos;
    in.close();

    std::streamoff expected = static_cast<std::streamoff>(w) * static_cast<std::streamoff>(h);
    if (dataBytes != expected) {
        std::cerr << "SaveAsPGM validation FAILED: data bytes=" << dataBytes << " expected=" << expected << "\n";
    } else {
        std::cout << "SaveAsPGM validation OK: wrote " << dataBytes << " bytes\n";
    }

    std::cout << "Image saved as " << filename << " (w=" << m_width << " h=" << m_height << " stride=" << m_stride << ")\n";
}