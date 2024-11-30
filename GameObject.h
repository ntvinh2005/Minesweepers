#include "UIelement.h"
#include <chrono>
#include <iostream>
#pragma once

class Mine {

};

class Tile {
private:
    bool hasMine; 
};

class Counter {

};

class Timer {
private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point pauseStartTime;
    std::chrono::duration<int> pausedDuration;
    bool running;
    bool paused;
public:
    Timer() {
        running = false;
    }
    void start();
    void stop();
    void reset();
    void pause();
    void resume();
    int getElapsedTimeInSeconds();
};

class TimerDisplay {
private:
    vector<Sprite> minSprites;
    vector<Sprite> secSprites;
    int DIGIT_WIDTH = 21;
    int DIGIT_HEIGHT = 32;
public:
    TimerDisplay() {};
    TimerDisplay(string texturePath);
    void update(int elapsedTime);
    void draw(sf::RenderWindow& window);
};