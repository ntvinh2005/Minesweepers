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
    minSprite.setTexture(texturePath);
    secSprite.setTexture(texturePath);

    minSprite.setPosition(50, 50);
    secSprite.setPosition(50 + 2 * DIGIT_WIDTH + 10, 50);
}

void TimerDisplay::update(int elapsedTime) {
    int minutes = elapsedTime / 60;
    int seconds = elapsedTime % 60;

    int minTens = minutes / 10;
    int minOnes = minutes % 10;

    int secTens = seconds / 10;
    int secOnes = seconds % 10;

    minSprite.getSprite().setTextureRect(sf::IntRect(minTens * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT));
    minSprite.getSprite().setTextureRect(sf::IntRect(minOnes * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT));

    secSprite.getSprite().setTextureRect(sf::IntRect(secTens * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT));
    secSprite.getSprite().setTextureRect(sf::IntRect(secOnes * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT));
}

void TimerDisplay::draw(sf::RenderWindow& window) {
    minSprite.draw(window);
    secSprite.draw(window);
}