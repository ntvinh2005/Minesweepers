#include "UIelement.h"

sf::Font& ResourceManager::getFont(std::string& fontPath) {
    static std::map<std::string, sf::Font> fonts;
    if (fonts.find(fontPath) == fonts.end()) {
        if (!fonts[fontPath].loadFromFile(fontPath)) {
            throw std::runtime_error("Failed to load font: " + fontPath);
        }
    }
    return fonts[fontPath];
}

std::map<std::string, sf::Texture> ResourceManager::textures;
sf::Texture& ResourceManager::getTexture(const std::string& texturePath){
    if (textures.find(texturePath) == textures.end()) {
        sf::Texture texture;
        if (!texture.loadFromFile(texturePath)) {
            std::cerr << "Error: Could not load texture from " << texturePath << std::endl;
            throw std::runtime_error("Error: Could not load texture from " + texturePath);
        }
        textures[texturePath] = std::move(texture);
    }
    return textures[texturePath];
}

Label::Label(string content, string fontPath, unsigned int fontSize, sf::Color color, sf::Vector2f pos) {
    font = &ResourceManager::getFont(fontPath);
    text.setFont(*font);
    text.setString(content);
    text.setCharacterSize(fontSize);
    text.setFillColor(color);
    text.setPosition(pos);
}

void Label::setPosition(sf::Vector2f pos) {
    text.setPosition(pos);
};
void Label::setText(string content) {
    text.setString(content);
};
void Label::setColor(sf::Color color) {
    text.setFillColor(color);
};
void Label::setFontSize(unsigned int fontSize) {
    text.setCharacterSize(fontSize);
};

void Label::setTextCenter(float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}
 
sf::Vector2f Label::getPosition() {
    return text.getPosition();
};
string Label::getText() {
    return text.getString();
};

void Label::draw(sf::RenderWindow& window) {
    window.draw(text);
};

InputBox::InputBox(sf::Vector2f size, sf::Vector2f position, string fontPath, unsigned int fontSize) {
    box.setSize(size);
    box.setPosition(position);
    box.setFillColor(sf::Color::White);
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(2);

    font = &ResourceManager::getFont(fontPath);

    text.setFont(*font);
    text.setCharacterSize(fontSize);
    text.setFillColor(sf::Color::Black);

    isActive = false;
    content = "";

    showCaret = true;
    caretBlinkTime = 0.5f;
}

void InputBox::update() {
    if (caretClock.getElapsedTime().asSeconds() > caretBlinkTime) {
        showCaret = !showCaret;
        caretClock.restart();
    }
}

void InputBox::setBoxCenter(float x, float y) {
    sf::FloatRect boxBounds = box.getLocalBounds();
    box.setOrigin(boxBounds.left + boxBounds.width / 2.0f, boxBounds.top + boxBounds.height / 2.0f);
    box.setPosition(sf::Vector2f(x, y));

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(0, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(sf::Vector2f(box.getPosition().x - box.getLocalBounds().width / 2.0f + 10, box.getPosition().y - box.getLocalBounds().height / 2.0f));
}

void InputBox::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        isActive = box.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y);
    }

    if (isActive && event.type == sf::Event::TextEntered) {
        if (event.text.unicode == '\b') {
            if (!content.empty()) {
                content.pop_back();
            }
        } else if (isalpha(event.text.unicode) && content.size() < 10) {
            if (content.size() == 0) content += toupper(static_cast<char>(event.text.unicode));
            else content += tolower(static_cast<char>(event.text.unicode));
        }
        text.setString(content);
    }
}
void InputBox::draw(sf::RenderWindow& window){
    window.draw(box);
    window.draw(text);
    if (isActive && showCaret) {
        sf::RectangleShape caret;
        caret.setSize(sf::Vector2f(2, text.getCharacterSize()));
        caret.setFillColor(sf::Color::Black);
        caret.setPosition(text.getPosition().x + text.getLocalBounds().width + 5, text.getPosition().y);
        window.draw(caret);
    }
}
string InputBox::getText() {
    return content;
}

void Sprite::setTexture (string texturePath) {
    try {
        texture = ResourceManager::getTexture(texturePath);
        sprite.setTexture(texture);
    } catch (const std::exception& e) {
        std::cerr << "Failed to set texture: " << e.what() << std::endl;
    }
}

void Sprite::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Sprite::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY);
}

void Sprite::setColor(sf::Color color) {
    sprite.setColor(color);
}

void Sprite::setOrigin(float x, float y) {
    sprite.setOrigin(x, y);
}

void Sprite::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Sprite& Sprite::getSprite() {
    return sprite;
}

Button::Button(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Font& font, 
        const std::string& label, const sf::Color& defaultColor, 
        const sf::Color& hoverColor, const sf::Color& activeColor) {
    box.setSize(size);
    box.setPosition(position);
    box.setFillColor(defaultColor);
    this->defaultColor = defaultColor;
    this->hoverColor = hoverColor;
    this->activeColor = activeColor;

    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
};

void Button::setOnClick(const std::function<void()>& callback) {
    onClick = callback;
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    if (box.getGlobalBounds().contains(mousePosF)) {
        box.setFillColor(hoverColor);

        // Check for click
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            box.setFillColor(activeColor);
        }
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            if (onClick) {
                onClick(); 
            }
        }
    } else {
        box.setFillColor(defaultColor);
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(text);
}