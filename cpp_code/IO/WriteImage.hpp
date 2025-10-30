#ifndef WRITE_IMAGE_HPP
#define WRITE_IMAGE_HPP
#include "DataObjects/Image.hpp"
#include "DataObjects/Contour.hpp"
#include <iostream>
#include <fstream>

class WriteImage
{
    public:
        WriteImage();
        void saveImage(const Image &image);

        void ContourOverLay(const Contour &contour, const Image &image);

    private:
        void SaveAsPGM(const std::vector<std::vector<uint8_t>> &image);
};

#endif // WRITE_IMAGE_HPP