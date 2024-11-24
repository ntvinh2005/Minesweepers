#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include "UIelement.h"
#include "UserData.h"
using namespace std;

class Window {
protected:
    int rowCount;
    int colCount;
    int height;
    int width;
    sf::RenderWindow window;
    UserData& userData;

public:
    Window(int _rowCount, int _colCount, UserData& _userData); 

    virtual ~Window() = default;

    virtual void handleEvent() = 0;
    virtual void update() = 0;
    virtual void render() = 0;

    void run();
    UserData& getUserData();
};

class WelcomeWindow : public Window {
private:
    Label welcomeLabel;
    Label nameInputLabel;
    InputBox nameInput;
public:
    WelcomeWindow(int _rowCount, int _colCount, UserData& _userData);
    void handleEvent() override;
    void update() override;
    void render() override;
};

class GameWindow : public Window {
public:
    GameWindow(int _rowCount, int _colCount, UserData& _userData);
    void handleEvent() override;
    void update() override;
    void render() override;
};

class LeaderboardWindow : public Window {
public:
    LeaderboardWindow(int _rowCount, int _colCount, UserData& _userData);
    void handleEvent() override;
    void update() override;
    void render() override;
};