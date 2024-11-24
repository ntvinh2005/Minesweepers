#include <iostream>
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "UserData.h"

int main() {
    UserData userData("");
    WelcomeWindow welcomeWindow(16, 25, userData);
    welcomeWindow.run();
    cout << userData.getName() << endl;
    GameWindow gameWindow(16, 25, userData);
    if (userData.getIsActive()) gameWindow.run();
    return 0;
}