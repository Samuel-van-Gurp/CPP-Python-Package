#ifndef IMAGE_HOLDER_BASE
#define IMAGE_HOLDER_BASE

template <typename T>
class ImageHolderBase
{
    public:
        virtual ~ImageHolderBase() = default;

        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;
        virtual Point getCenter() const = 0;
        virtual bool empty() const = 0;

    protected:
        int m_width;
        int m_height;
        int m_stride;
};

#endif // IMAGE_HOLDER_BASE 
