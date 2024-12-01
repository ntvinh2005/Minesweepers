#include "UIelement.h"
#include <chrono>
#include <iostream>
#include <random>
#include <algorithm>
#pragma once

class Tile {
private:
    bool hasMine; 
    bool hasFlag;
    bool isRevealed;
    vector<Tile*> adjacentTiles;
    int adjacentMines;
    Sprite sprite;
    Sprite mineSprite;
    sf::Vector2f position;
    int tileSize;
public:
    Tile(bool _hasMine, sf::Vector2f _position);
    void update();
    void reveal();
    void draw(sf::RenderWindow& window);
    void reset();
    void setMine();
    bool isMine();
    bool checkRevealed();
    bool checkFlag();
    void setAdjacentMineCount(int mineCount);
    void setAdjacentTiles(vector<Tile*> _adjacentTiles);
    int getAdjacentMines();
    vector<Tile*>& getAdjacentTiles();
};

class Board {
private:
    vector<vector<Tile>> tiles;
    int rowCount;
    int colCount;
    int mineCount;
public:
    Board();
    void revealAdjacent(Tile* tile);
    void draw(sf::RenderWindow& window);
    void assignMines();
    void findAdjacent();
    bool checkWin();
    void reset();
    void update();
};

class Counter {
private:
    int mineLeftNum = 50;
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