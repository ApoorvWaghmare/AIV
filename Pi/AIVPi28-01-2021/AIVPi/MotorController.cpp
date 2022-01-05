#include "MotorController.h"

MotorController::MotorController()
{
    //ctor
}

void MotorController::setGPIOPins()
{
    //for motor 1
    pinMode(M1_1, OUTPUT);
	pinMode(M1_2, OUTPUT);
	//for motor 2
	pinMode(M2_1, OUTPUT);
	pinMode(M2_2, OUTPUT);
}

void MotorController::m1Forward()
{
    digitalWrite(M1_1, 0);
    digitalWrite(M1_2, 1);
}

void MotorController::m1Backward()
{
    digitalWrite(M1_1, 1);
    digitalWrite(M1_2, 0);
}

void MotorController::m1Stop()
{
    digitalWrite(M1_1, 0);
    digitalWrite(M1_2, 0);
}

void MotorController::m2Forward()
{
    digitalWrite(M2_1, 1);
    digitalWrite(M2_2, 0);
}

void MotorController::m2Backward()
{
    digitalWrite(M2_1, 0);
    digitalWrite(M2_2, 1);
}

void MotorController::m2Stop()
{
    digitalWrite(M2_1, 0);
    digitalWrite(M2_2, 0);
}

MotorController::~MotorController()
{
    //dtor
    // stop bot
    digitalWrite(M1_1, 0);
    digitalWrite(M1_2, 0);
    digitalWrite(M2_1, 0);
    digitalWrite(M2_2, 0);
}
