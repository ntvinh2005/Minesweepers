#include "GameObject.h"

Tile::Tile(bool _hasMine, sf::Vector2f _position) {
    hasMine = _hasMine;
    hasFlag = false;
    isRevealed = false;
    
    position = _position;
    revealSprite.setTexture("files/images/tile_revealed.png");
    revealSprite.setPosition(position.x, position.y);
    numberSprite.setTexture("files/images/tile_hidden.png");
    numberSprite.setPosition(position.x, position.y);
    mineSprite.setTexture("files/images/tile_hidden.png");
    mineSprite.setPosition(position.x, position.y);
    mineSprite.setColor(sf::Color(255, 255, 255, 0));
    tileSize = 32;
}

void Tile::reveal() {
    isRevealed = true;
}

void Tile::update() {
    if (!numberSprite.getSprite().getTexture()) {
        std::cout << "Failed to load texture!" << std::endl;
    }

    if (isRevealed) {
        if (hasMine) {
            mineSprite.setColor(sf::Color(255, 255, 255, 255));
            mineSprite.setTexture("files/images/mine.png");
        } else if (adjacentMines > 0) {
            numberSprite.setTexture("files/images/number_" + std::to_string(adjacentMines) + ".png");
            numberSprite.setColor(sf::Color(255, 255, 255, 255));
        } else {
            numberSprite.setTexture("files/images/tile_revealed.png");
            numberSprite.setColor(sf::Color(255, 255, 255, 0));
        }
    }

    if (hasFlag) {
        mineSprite.setColor(sf::Color(255, 255, 255, 255));
        mineSprite.setTexture("files/images/flag.png");
    }
    else if (!hasMine) {
        mineSprite.setColor(sf::Color(255, 255, 255, 0));
        mineSprite.setTexture("files/images/tile_hidden.png");
    }
}

void Tile::draw(sf::RenderWindow& window) {
    revealSprite.draw(window);
    numberSprite.draw(window);
    if (hasMine || hasFlag) mineSprite.draw(window);
}

void Tile::reset() {
    hasMine = false;
    hasFlag = false;
    isRevealed = false;
    numberSprite.setTexture("files/images/tile_hidden.png");
    numberSprite.setColor(sf::Color(255, 255, 255, 255));
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

void Tile::toggleFlag() {
    if (!isRevealed) {
        hasFlag = !hasFlag;
    }
}

Board::Board(int _rowCount, int _colCount, int _mineCount) {
    rowCount = _rowCount;
    colCount = _colCount;
    mineCount = _mineCount;
    tiles.resize(rowCount);
    for (int i = 0; i < rowCount; ++i) {
        tiles[i].resize(colCount);
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
    for (auto& row : tiles) {
        for (auto& tile : row) {
            tile.draw(window);
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
        int row = index / colCount;
        int col = index % colCount;
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
                        }
                        adjTiles.push_back(&tiles[dx][dy]);
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

void Board::checkWin() {
    winStatus = 1;
    bool allRevealed = true;
    for (auto& row : tiles) {
        for (auto& tile : row) {
            if (tile.isMine() && tile.checkRevealed() && !debugMode) {
                winStatus = 0;
                return;
            }
            if (!tile.checkRevealed() && !tile.checkFlag()) {
                allRevealed = false;
            }
        }
    }
    if (allRevealed) winStatus = 2;
}

int Board::getWinStatus() {
    return winStatus;
}

void Board::reset() {
    debugMode = false;
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

void Board::handleClick(sf::Vector2i mousePosition, bool isRightClick) {
    int col = mousePosition.x / 32;
    int row = mousePosition.y / 32;

    if (row >= 0 && row < rowCount && col >= 0 && col < colCount) {
        Tile& clickedTile = tiles[row][col];
        
        if (isRightClick) {
            if (!clickedTile.checkRevealed()) {
                clickedTile.toggleFlag();
            }
        } else {
            if (!clickedTile.checkFlag()) {
                if (clickedTile.isMine()) {
                    clickedTile.reveal();
                    std::cout << "Game Over!" << std::endl;
                } else {
                    revealAdjacent(&clickedTile);
                }
            }
        }
    }
}

int Board::countFlag() {
    int flagCount = 0;
    for (auto& row : tiles) {
        for (auto& tile : row) {
            if (tile.checkFlag()) flagCount++;
        }
    }
    return flagCount;
}

void Board::turnonDebugMode() {
    debugMode = true;
}

vector<vector<Tile>>& Board::getTiles() {
    return tiles;
};

Counter::Counter(std::string texturePath, float x, float y) {
    for (int i = 0; i < 3; ++i) {
        Sprite sprite;
        sprite.setTexture(texturePath);
        sprite.setPosition(x + i * DIGIT_WIDTH, y);
        digitSprites.push_back(sprite);
    }
    count = 50; 
    updateDisplay();
}

void Counter::setCount(int flags) {
    count = 50 - flags;
    updateDisplay();
}

void Counter::updateDisplay() {
    int displayCount = std::abs(count); 
    int hundreds = (displayCount / 100) % 10;
    int tens = (displayCount / 10) % 10;
    int ones = displayCount % 10;

    digitSprites[0].getSprite().setTextureRect(sf::IntRect(hundreds * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT));
    digitSprites[1].getSprite().setTextureRect(sf::IntRect(tens * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT));
    digitSprites[2].getSprite().setTextureRect(sf::IntRect(ones * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT));
    if (count < 0) {
        digitSprites[0].getSprite().setTextureRect(sf::IntRect(10 * DIGIT_WIDTH, 0, DIGIT_WIDTH, DIGIT_HEIGHT)); // Assuming 10th digit is the "-" sign
    }
}

void Counter::draw(sf::RenderWindow& window) {
    for (auto& sprite : digitSprites) {
        sprite.draw(window);
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

bool Timer::checkPauseStatus() {
    return paused;
}

TimerDisplay::TimerDisplay(string texturePath, float x, float y) {
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

    minSprites[0].setPosition(x, y);  
    minSprites[1].setPosition(x + DIGIT_WIDTH, y);  

    secSprites[0].setPosition(x + 2 * DIGIT_WIDTH + 10, y); 
    secSprites[1].setPosition(x + 3 * DIGIT_WIDTH + 10, y);
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