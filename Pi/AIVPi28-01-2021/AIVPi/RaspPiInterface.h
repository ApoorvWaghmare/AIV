#ifndef RASPPIINTERFACE_H
#define RASPPIINTERFACE_H

#include<wiringPi.h>

class RaspPiInterface
{
    public:
        RaspPiInterface();
        virtual ~RaspPiInterface();
        virtual void setGPIOPins() = 0;

    protected:

    private:
};

#endif // RASPPIINTERFACE_H
