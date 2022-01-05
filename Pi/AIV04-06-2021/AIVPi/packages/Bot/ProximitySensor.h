#ifndef PROXIMITYSENSOR_H
#define PROXIMITYSENSOR_H

#define TRIG 23
#define ECHO 22

class ProximitySensor
{
    public:
        ProximitySensor();
        virtual ~ProximitySensor();

        double getDistance();

    protected:

    private:
        // setup
        void setGPIOPins();
};

#endif // PROXIMITYSENSOR_H
