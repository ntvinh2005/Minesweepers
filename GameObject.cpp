#include "GameObject.h"

Tile::Tile(bool _hasMine, sf::Vector2f _position) {
    hasMine = _hasMine;
    hasFlag = false;
    isRevealed = false;
    sprite.setTexture("files/images/tile_hidden.png");
    position = _position;
    sprite.setPosition(position.x, position.y);
    mineSprite.setTexture("files/images/tile_hidden.png");
    mineSprite.setPosition(position.x, position.y);
    tileSize = 32;
}

void Tile::reveal() {
    isRevealed = true;
}

void Tile::update() {
    if (isRevealed) {
        if (hasMine) {
            mineSprite.setColor(sf::Color(255, 255, 255, 255));
            mineSprite.setTexture("files/images/mine.png");
        } else if (adjacentMines > 0) {
            sprite.setTexture("files/images/number_" + std::to_string(adjacentMines) + ".png");
        } else {
            sprite.setTexture("files/images/tile_revealed.png");
        }
    }

    if (hasFlag) {
        mineSprite.setColor(sf::Color(255, 255, 255, 255));
        mineSprite.setTexture("files/images/flag.png");
    }
}

void Tile::draw(sf::RenderWindow& window) {
    sprite.draw(window);
    if (hasMine || hasFlag) mineSprite.draw(window);
}

void Tile::reset() {
    hasMine = false;
    hasFlag = false;
    isRevealed = false;
    sprite.setTexture("files/images/tile_hidden.png");
    mineSprite.setColor(sf::Color(255, 255, 255, 0));
}

void Tile::setMine() {
    hasMine = true;
}

bool Tile::isMine() {
    return hasMine;
}

bool Tile::checkRevealed() {
    return isRevealed;
}

bool Tile::checkFlag() {
    return hasFlag;
}

void Tile::setAdjacentMineCount(int mineCount) {
    adjacentMines = mineCount;
}

void Tile::setAdjacentTiles(vector<Tile*> _adjacentTiles) {
    adjacentTiles = _adjacentTiles;
};

int Tile::getAdjacentMines() {
    return adjacentMines;
}

vector<Tile*>& Tile::getAdjacentTiles() {
    return adjacentTiles;
}

Board::Board() {
    rowCount = 16;
    colCount = 25;
    mineCount = 50;
    tiles.resize(rowCount, vector<Tile>(colCount, Tile(false, sf::Vector2f(0, 0))));
    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < colCount; ++j) {
            bool hasMine = false;
            tiles[i][j] = Tile(hasMine, sf::Vector2f(j * 32, i * 32));
        }
    }
    cout << "Board created" << endl;
    assignMines();
    findAdjacent();
}

void Board::draw(sf::RenderWindow& window) {
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            tiles[row][col].draw(window);
        }
    }
}

void Board::assignMines() {
    vector<int> availableTiles;
    for (int i = 0; i < rowCount * colCount; ++i) {
        availableTiles.push_back(i);
    }
    random_device rd;
    mt19937 gen(rd());
    shuffle(availableTiles.begin(), availableTiles.end(), gen);
    for (int i = 0; i < mineCount; i++) {
        int index = availableTiles[i];
        int row = index / rowCount;
        int col = index % rowCount;
        tiles[row][col].setMine();
    }
    cout << "Assigned Mine" << endl;
}

void Board::findAdjacent() {
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            if (tiles[i][j].isMine()) continue;
            int adjacentMineCount = 0;
            vector<Tile*> adjTiles;
            for (int x = -1; x <= 1; x ++) {
                for (int y = -1; y <= 1; y ++) {
                    if (x == 0 && y == 0) continue;
                    int dx = i + x;
                    int dy = j + y;
                    if (dx >= 0 && dx < rowCount && dy >= 0 && dy < colCount) {
                        if (tiles[dx][dy].isMine()) {
                            adjacentMineCount ++;
                            adjTiles.push_back(&tiles[dx][dy]);
                        }
                    }
                }
            }
            tiles[i][j].setAdjacentMineCount(adjacentMineCount);
            tiles[i][j].setAdjacentTiles(adjTiles);
        }
    }
    cout << "Found Adjacent" << endl;
}

void Board::revealAdjacent(Tile* tile) {
    // Base case
    if (tile->checkRevealed() || tile->isMine() || tile->checkFlag()) {
        return;
    }
    tile->reveal();

    if (tile->getAdjacentMines() > 0) {
        return;
    }
    for (Tile* adjacent : tile->getAdjacentTiles()) {
        revealAdjacent(adjacent);
    }
}

bool Board::checkWin() {
    bool won = true;
    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < colCount; ++j) {
            if (!tiles[i][j].isMine() && !tiles[i][j].checkRevealed()) {
                return false;
            }
        }
    }
    return true;
}

void Board::reset() {
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            tiles[row][col].reset();
        }
    }
    assignMines();
    findAdjacent();
}

void Board::update() {
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j ++) {
            tiles[i][j].update();
        }
    }
}

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

    for (auto& sprite : minSprites) {
        sprite.setTexture(texturePath);
    }

    for (auto& sprite : secSprites) {
        sprite.setTexture(texturePath);
    }

    minSprites[0].setPosition(800, 562);  
    minSprites[1].setPosition(800 + DIGIT_WIDTH, 562);  

    secSprites[0].setPosition(800 + 2 * DIGIT_WIDTH + 10, 562); 
    secSprites[1].setPosition(800 + 3 * DIGIT_WIDTH + 10, 562);
}

void TimerDisplay::update(int elapsedTime) {
    int minutes = elapsedTime / 60;
    int seconds = elapsedTime % 60;

    int minTens = minutes / 10;
    int minOnes = minutes % 10;

    int secTens = seconds / 10;
    int secOnes = seconds % 10;

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