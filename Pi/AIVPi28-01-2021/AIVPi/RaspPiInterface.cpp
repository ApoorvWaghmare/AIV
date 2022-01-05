#include "RaspPiInterface.h"

RaspPiInterface::RaspPiInterface()
{
    //ctor
    wiringPiSetup();  //setup wiring pi
}

RaspPiInterface::~RaspPiInterface()
{
    //dtor
}
