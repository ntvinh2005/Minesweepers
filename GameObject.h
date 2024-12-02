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
    Sprite revealSprite;
    Sprite numberSprite;
    Sprite mineSprite;
    sf::Vector2f position;
    int tileSize;
public:
    Tile() {};
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
    void toggleFlag();
};

class Board {
private:
    vector<vector<Tile>> tiles;
    int rowCount;
    int colCount;
    int mineCount;
    int winStatus; // 0 is lose, 1 is undecided, 2 is win.
    bool debugMode;
public:
    Board() {};
    Board(int _rowCount, int _colCount, int _minCount);
    void revealAdjacent(Tile* tile);
    void draw(sf::RenderWindow& window);
    void assignMines();
    void findAdjacent();
    void checkWin();
    int getWinStatus();
    void reset();
    void update();
    void handleClick(sf::Vector2i mousePosition, bool isRightClick);
    vector<vector<Tile>>& getTiles();
    int countFlag();
    void turnonDebugMode();
};

class Counter {
private:
    std::vector<Sprite> digitSprites; 
    int count;       
    int DIGIT_WIDTH = 21;
    int DIGIT_HEIGHT = 32;                 

public:
    Counter() {};
    Counter(std::string texturePath, float x, float y);
    void setCount(int flags);
    void updateDisplay();
    void draw(sf::RenderWindow& window);
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
    bool checkPauseStatus();
};

class TimerDisplay {
private:
    vector<Sprite> minSprites;
    vector<Sprite> secSprites;
    int DIGIT_WIDTH = 21;
    int DIGIT_HEIGHT = 32;
public:
    TimerDisplay() {};
    TimerDisplay(string texturePath, float x, float y);
    void update(int elapsedTime);
    void draw(sf::RenderWindow& window);
};