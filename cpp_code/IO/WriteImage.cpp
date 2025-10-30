#include "DataObjects/Image.hpp"
#include "WriteImage.hpp"

WriteImage::WriteImage()
{
}

void WriteImage::saveImage(const Image &image)
{
    SaveAsPGM(image.GetImageVector());
}

void WriteImage::ContourOverLay(const Contour& contour, const Image &image) 
{
    // scale image intensity down to make the contour more visible
    auto ImageCopy = image.scaleIntensity(2);

    for (size_t i = 0; i < contour.Size(); ++i)
    {
        const Point& pt = contour[i];
        if (pt.Y >= 0 && pt.Y < image.GetHeight() && pt.X >= 0 && pt.X < image.GetWidth())
        {
            ImageCopy[pt.Y][pt.X] = 255; // set contour pixel to white
        }
    }

    SaveAsPGM(ImageCopy);
    std::cout << "Contour overlay image saved as contour_overlay.pgm\n";
}

// function save intermediate image as PGM file, for debugging purpose
void WriteImage::SaveAsPGM(const std::vector<std::vector<uint8_t>>& image)
{
    // Validate input image
    if (image.empty()) {
        std::cerr << "SaveAsPGM error: image is empty\n";
        return;
    }
    int h = static_cast<int>(image.size());
    int w = static_cast<int>(image[0].size());
    if (w <= 0 || h <= 0) {
        std::cerr << "SaveAsPGM error: invalid image dimensions (w=" << w << " h=" << h << ")\n";
        return;
    }
    for (int y = 0; y < h; ++y) {
        if (static_cast<int>(image[y].size()) != w) {
            std::cerr << "SaveAsPGM error: inconsistent row width at row " << y
                      << " (expected " << w << ", got " << image[y].size() << ")\n";
            return;
        }
    }

    std::string filename = "C:\\Users\\svangurp\\Desktop\\CPP_PY_package\\DebugImage\\gradient.pgm";

    // Build a single contiguous buffer from image
    std::vector<uint8_t> buffer;
    buffer.reserve(static_cast<size_t>(w) * static_cast<size_t>(h));
    for (int y = 0; y < h; ++y) {
        buffer.insert(buffer.end(), image[y].begin(), image[y].begin() + w);
    }

    // debug: range check
    uint8_t minv = 255, maxv = 0;
    for (uint8_t v : buffer) { if (v < minv) minv = v; if (v > maxv) maxv = v; }
    std::cout << "SaveAsPGM: writing buffer (w=" << w << " h=" << h << ") min=" << int(minv) << " max=" << int(maxv) << "\n";

    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "SaveAsPGM error: failed to open '" << filename << "' for writing\n";
        return;
    }

    out << "P5\n" << w << " " << h << "\n255\n";
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
    int w_read = 0, h_read = 0, maxv_read = 0;
    in >> w_read >> h_read >> maxv_read;
    // consume single whitespace char after header (per PGM spec)
    in.get();

    std::streampos pos = in.tellg();
    in.seekg(0, std::ios::end);
    std::streamoff fileSize = in.tellg();
    std::streamoff dataBytes = fileSize - pos;
    in.close();

    std::streamoff expected = static_cast<std::streamoff>(w_read) * static_cast<std::streamoff>(h_read);
    if (dataBytes != expected) {
        std::cerr << "SaveAsPGM validation FAILED: data bytes=" << dataBytes << " expected=" << expected << "\n";
    } else {
        std::cout << "SaveAsPGM validation OK: wrote " << dataBytes << " bytes\n";
    }

    std::cout << "Image saved as " << filename << " (w=" << w << " h=" << h << ")\n";
}


