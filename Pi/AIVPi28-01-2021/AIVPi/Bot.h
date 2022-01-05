#ifndef BOT_H
#define BOT_H


class Bot
{
    public:
        Bot();
        virtual ~Bot();
        double getWheelRadius();
        double getWheelBase();

    protected:

    private:
        //all distances in meters
        double m_wheelRadius = 0.03835;
        double m_wheelBase = 0.2105;
};

#endif // BOT_H
