#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;

class ResourceManager {
public:
    static sf::Font& getFont(std::string& fontPath);
};

class Label {
private:
    sf::Text text;
    sf::Font* font;

public:
    Label() {};
    Label(string content, string fontPath, unsigned int fontSize, sf::Color color, sf::Vector2f pos);

    void setPosition(sf::Vector2f pos);
    void setText(string content);
    void setColor(sf::Color color);
    void setFontSize(unsigned int fontSize);
    void setTextCenter(float x, float y);

    sf::Vector2f getPosition();
    string getText();

    void draw(sf::RenderWindow& window);
};

class InputBox {
private:
    sf::RectangleShape box;
    sf::Text text;
    sf::Font* font;
    string content;
    bool isActive;
    bool showCaret;
    sf::Clock caretClock;
    float caretBlinkTime;

public:
    InputBox() {};
    InputBox(sf::Vector2f size, sf::Vector2f position, string fontPath, unsigned int fontSize);
    void update();
    void setBoxCenter(float x, float y);
    void handleEvent(sf::Event& event);
    void draw(sf::RenderWindow& window);
    string getText();
};