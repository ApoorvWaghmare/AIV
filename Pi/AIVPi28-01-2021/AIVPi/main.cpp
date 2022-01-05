#include <iostream>

#include "BotController.h"

//using namespace std;

int main()
{
    BotController botController;

    //botController.connectClient();

    int direction;
    double distance;

    cout << "Enter direction : ";
    cin >> direction;
    cout << endl;

    cout << "Enter distance : ";
    cin >> distance;
    cout << endl;

    botController.tempStartOperations(direction, distance);

    return 0;
}
