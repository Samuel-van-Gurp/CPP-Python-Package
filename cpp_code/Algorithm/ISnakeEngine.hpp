class ISnakeEngine
{
    public:
        ISnakeEngine() {} 

        virtual void RunSnake(int iterations) = 0;
        virtual void EvolveContour() = 0;
};