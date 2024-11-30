#include "GameObject.h"

void Timer::start() {
    running = true;
    paused = false;
    pausedDuration = std::chrono::seconds(0);
    startTime = std::chrono::high_resolution_clock::now();
}

void Timer::stop() {
    running = false;
    paused = false;
}

void Timer::reset() {
    running = false;
    paused = false;
    pausedDuration = std::chrono::seconds(0);
}

void Timer::pause() {
    if (running && !paused) {
        paused = true;
        pauseStartTime = std::chrono::high_resolution_clock::now();
    }
}

void Timer::resume() {
    if (running && paused) {
        paused = false;
        auto now = std::chrono::high_resolution_clock::now();
        pausedDuration += std::chrono::duration_cast<std::chrono::seconds>(now - pauseStartTime);
    }
}

int Timer::getElapsedTimeInSeconds() {
    if (!running) {
        return 0;
    }

    auto now = paused ? pauseStartTime : std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count() - pausedDuration.count();
}

TimerDisplay::TimerDisplay(string texturePath) {
    minSprites.push_back(Sprite());
    minSprites.push_back(Sprite());
    secSprites.push_back(Sprite());
    secSprites.push_back(Sprite());

    // Set texture for all the sprites
    for (auto& sprite : minSprites) {
        sprite.setTexture(texturePath);
    }

    for (auto& sprite : secSprites) {
        sprite.setTexture(texturePath);
    }

    // Position the sprites (adjust based on your layout)
    minSprites[0].setPosition(50, 50);  // Tens place of minutes
    minSprites[1].setPosition(50 + DIGIT_WIDTH, 50);  // Ones place of minutes

    secSprites[0].setPosition(50 + 2 * DIGIT_WIDTH + 10, 50);  // Tens place of seconds
    secSprites[1].setPosition(50 + 3 * DIGIT_WIDTH + 10, 50);
}

void TimerDisplay::update(int elapsedTime) {
    int minutes = elapsedTime / 60;
    int seconds = elapsedTime % 60;

    // Extract the tens and ones digits for minutes
    int minTens = minutes / 10;
    int minOnes = minutes % 10;

    // Extract the tens and ones digits for seconds
    int secTens = seconds / 10;
    int secOnes = seconds % 10;

    // Set the textures for the digits (digits are placed horizontally)
    minSprites[0].getSprite().setTextureRect(sf::IntRect(minTens * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT));
    minSprites[1].getSprite().setTextureRect(sf::IntRect(minOnes * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT));

    secSprites[0].getSprite().setTextureRect(sf::IntRect(secTens * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT));
    secSprites[1].getSprite().setTextureRect(sf::IntRect(secOnes * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT));
}

void TimerDisplay::draw(sf::RenderWindow& window) {
    for (auto& sprite : minSprites) {
        sprite.draw(window);
    }

    for (auto& sprite : secSprites) {
        sprite.draw(window);
    }
}