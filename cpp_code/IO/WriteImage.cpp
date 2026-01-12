#include "ImageProcessing/ImageProcessorFacade.hpp"
#include "WriteImage.hpp"

WriteImage::WriteImage()
{
}

void WriteImage::saveImage(const ImageHolder<float> &image, const std::string &filename)
{
    SaveAsPGM(image, filename);
}

void WriteImage::ContourOverLay(const Contour& contour, const ImageHolder<float> &image, const ImageProcessorFacade &processor) 
{
    // scale image intensity down to make the contour more visible
    ImageHolder<float> ImageCopy = image;
    processor.scaleIntensity(2, ImageCopy);

    for (size_t i = 0; i < contour.Size(); ++i)
    {
        const Point& pt = contour[i];
        if (pt.Y >= 0 && pt.Y < image.getHeight() && pt.X >= 0 && pt.X < image.getWidth())
        {
            ImageCopy.setPixel(pt.X, pt.Y, 1); // set contour pixel to white
        }
    }

    SaveAsPGM(ImageCopy , "contour_overlay");
    std::cout << "Contour overlay image saved as contour_overlay.pgm\n";
}

// function save intermediate image as PGM file, for debugging purpose
void WriteImage::SaveAsPGM(const ImageHolder<float>& image, const std::string& fname)
{
    std::string filename = "C:\\Users\\svangurp\\Desktop\\CPP_PY_package\\DebugImage\\" + fname + ".pgm";
    // Validate input image
    if (image.empty()) {
        std::cerr << "SaveAsPGM error: image is empty\n";
        return;
    }
    int h = static_cast<int>(image.getHeight());
    int w = static_cast<int>(image.getWidth());
    if (w <= 0 || h <= 0) {
        std::cerr << "SaveAsPGM error: invalid image dimensions (w=" << w << " h=" << h << ")\n";
        return;
    }
    for (int y = 0; y < h; ++y) {
        if (static_cast<int>(image.getWidth()) != w) {
            std::cerr << "SaveAsPGM error: inconsistent row width at row " << y
                      << " (expected " << w << ", got " << image.getWidth() << ")\n";
            return;
        }
    }

    // Build a single contiguous buffer from image
    // build float buffer (values expected in 0..255)
    std::vector<float> buffer;
    buffer.reserve(static_cast<size_t>(w) * static_cast<size_t>(h));
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            buffer.push_back(image.getPixel(x, y));
        }
    }

    // debug: range check on float values
    float minv = std::numeric_limits<float>::infinity();
    float maxv = -std::numeric_limits<float>::infinity();
    for (float v : buffer) { if (v < minv) minv = v; if (v > maxv) maxv = v; }
    std::cout << "SaveAsPGM: writing buffer (w=" << w << " h=" << h << ") min=" << minv << " max=" << maxv << "\n";

    // decide if we need to scale floats from [0,1] -> [0,255]
    float scale = 1.0f;
    if (maxv <= 1.0f && minv >= 0.0f) {
        scale = 255.0f;
        std::cout << "SaveAsPGM: detected float range [0,1], scaling by 255\n";
    }

    // convert floats to bytes with scaling and clamping
    std::vector<uint8_t> outbuf;
    outbuf.reserve(buffer.size());
    for (float v : buffer) {
        float vs = v * scale;
        int vi = static_cast<int>(std::round(vs));
        if (vi < 0) vi = 0;
        if (vi > 255) vi = 255;
        outbuf.push_back(static_cast<uint8_t>(vi));
    }

    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "SaveAsPGM error: failed to open '" << filename << "' for writing\n";
        return;
    }

    out << "P5\n" << w << " " << h << "\n255\n";
    out.write(reinterpret_cast<const char*>(outbuf.data()), static_cast<std::streamsize>(outbuf.size()));
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


