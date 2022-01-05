#include <iostream>

#include "Bot.h"

using namespace std;

int main()
{
    Bot bot;

    //bot.connectClient();

    int direction;
    double distance;

    cout << "Enter direction : ";
    cin >> direction;
    cout << endl;

    cout << "Enter distance : ";
    cin >> distance;
    cout << endl;

    bot.tempStartOperations(direction, distance);

    return 0;
}
