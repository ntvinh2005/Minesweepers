#include <string>
#include <iostream>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <functional>
using namespace std;
#pragma once

class ResourceManager {
private:
    static std::map<std::string, std::shared_ptr<sf::Texture>> textures;
public:
    static sf::Font& getFont(std::string& fontPath);
    static std::shared_ptr<sf::Texture> getTexture(const std::string& texturePath);
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

class Sprite {
private:
    sf::Sprite sprite;
    sf::Texture texture;
public:
    Sprite() {};
    void setTexture (string texturePath);
    void setPosition(float x, float y);
    void setScale(float scaleX, float scaleY);
    void setColor(sf::Color color);

    void setOrigin(float x, float y);

    void draw(sf::RenderWindow& window);

    sf::Sprite& getSprite();
};

class Button {
private:
    sf::RectangleShape box;
    sf::Text text;
    sf::Color defaultColor;
    sf::Color hoverColor;
    sf::Color activeColor;
    std::function<void()> onClick;
public:
    Button(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Font& font, 
           const std::string& label, const sf::Color& defaultColor, 
           const sf::Color& hoverColor, const sf::Color& activeColor);
    void setOnClick(const std::function<void()>& callback);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
};