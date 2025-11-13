#ifndef ISNAKEENGINE_HPP
#define ISNAKEENGINE_HPP

class ISnakeEngine
{
    public:
        ISnakeEngine() {}

        virtual void RunSnake(int iterations) = 0;
        virtual void EvolveContour() = 0;
};

#endif // ISNAKEENGINE_HPP