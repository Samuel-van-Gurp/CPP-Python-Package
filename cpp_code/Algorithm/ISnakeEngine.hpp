#ifndef ISNAKEENGINE_HPP
#define ISNAKEENGINE_HPP

class ISnakeEngine
{
    public:
        ISnakeEngine() {}

        virtual Point* RunSnake(int iterations) = 0;
        virtual bool EvolveContour() = 0;
};

#endif // ISNAKEENGINE_HPP