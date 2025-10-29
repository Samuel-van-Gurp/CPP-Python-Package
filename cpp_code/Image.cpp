#include "Image.hpp"

Image::Image(const uint8_t *data, int width, int height, int stride)
    : m_data(data), m_width(width), m_height(height), m_stride(stride)
{
    ReconstructImage();
}

// Image::~Image() {
// }

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

// void Image::SaveAsPGM() 
// {
//     std::string filename = "output.pgm";
//     const auto& image = m_image;
//     int height = image.size();
//     int width = image[0].size();
//     std::ofstream out(filename, std::ios::binary);
//     out << "P5\n" << width << " " << height << "\n255\n";
//     for (const auto& row : image) {
//         out.write(reinterpret_cast<const char*>(row.data()), width);
//     }
//     out.close();
//     std::cout << "Image saved as " << filename << "\n";
// }

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
    if (m_stride < m_width) {
        std::cerr << "SaveAsPGM warning: stride (" << m_stride << ") < width (" << m_width << ")\n";
    }

    std::string filename = "output.pgm";
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "SaveAsPGM error: failed to open '" << filename << "' for writing\n";
        return;
    }

    // Write PGM header
    out << "P5\n" << m_width << " " << m_height << "\n255\n";
    if (!out) {
        std::cerr << "SaveAsPGM error: failed writing header\n";
        return;
    }

    // Write each row directly from the original buffer using stride
    for (int y = 0; y < m_height; ++y) {
        const char* rowPtr = reinterpret_cast<const char*>(m_data + (size_t)y * m_stride);
        out.write(rowPtr, m_width);
        if (!out) {
            std::cerr << "SaveAsPGM error: write failed at row " << y << "\n";
            out.close();
            return;
        }
    }

    out.close();
    std::cout << "Image saved as " << filename << " (w=" << m_width << " h=" << m_height << " stride=" << m_stride << ")\n";
}