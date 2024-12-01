#include "Window.h"

Window::Window(int _rowCount, int _colCount, UserData& _userData)
    : userData(_userData), rowCount(_rowCount), colCount(_colCount) {}

void Window::run() {
    while (window.isOpen()) {
        handleEvent();
        update();
        render();
    }
}

UserData& Window::getUserData() {
    return userData;
}

WelcomeWindow::WelcomeWindow(int _rowCount, int _colCount, UserData& _userData)
    : Window(_rowCount, _colCount, _userData) {
    this->height = (_rowCount * 32) + 100;
    this->width = _colCount * 32;
    window.create(sf::VideoMode(width, height), "Welcome Window");
    sf::Vector2f position;
    position.x = width/2;
    position.y = height/2 - 150;
    this->welcomeLabel = Label("WELCOME TO MINESWEEPERS!", "files/font.ttf", 24, sf::Color::White, position);
    this->welcomeLabel.setTextCenter(position.x, position.y);
    position.y = height/2 - 75;
    this->nameInputLabel = Label("Enter your name:", "files/font.ttf", 20, sf::Color::White, position);
    this->nameInputLabel.setTextCenter(position.x, position.y);
    sf::Vector2f size;
    size.x = 150;
    size.y = 20;
    position.y = height/2 - 45;
    this->nameInput = InputBox(size, position, "files/font.ttf", 18);
    nameInput.setBoxCenter(position.x, position.y);
}

void WelcomeWindow::handleEvent() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            userData.switchIsActive();
            cout << userData.getIsActive();
            window.close();
        }
            
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter && nameInput.getText().size() != 0) {
                userData.setName(nameInput.getText());
                window.close();
            }
        }
        nameInput.handleEvent(event);
    }
}

void WelcomeWindow::update() {
    nameInput.update();
}

void WelcomeWindow::render() {
    window.clear(sf::Color::Blue);
    welcomeLabel.draw(window);
    nameInputLabel.draw(window);
    nameInput.draw(window);
    window.display();
}

GameWindow::GameWindow(int _rowCount, int _colCount, UserData& _userData) : Window(_rowCount, _colCount, _userData) {
    this->height = (_rowCount * 32)+100;
    this->width = _colCount * 32;
    window.create(sf::VideoMode(width, height), "Game Window");
    timer.start();
    timerDisplay = TimerDisplay("files/images/digits.png", width - 100, height - 50);
    counter = Counter("files/images/digits.png", 100, height - 50);
    board = Board(_rowCount, _colCount, 50);
    resetButton =  Button(
        sf::Vector2f(50, 50),                    
        sf::Vector2f(200, height - 70),          // Button position
        "files/images/face_happy.png",         
        sf::Color(200, 200, 200),              
        sf::Color(150, 150, 150),               
        sf::Color(100, 100, 100)              
    );
    resetButton.setOnClick([&]() {
        resetOnClick(timer, board);
    });
    debugButton = Button(
        sf::Vector2f(50, 50),                    
        sf::Vector2f(width - 300, height - 70),          // Button position
        "files/images/debug.png",         
        sf::Color(200, 200, 200),               
        sf::Color(150, 150, 150),              
        sf::Color(100, 100, 100)               
    );
    debugButton.setOnClick([&]() {
        debugOnClick(board);
    });
    pauseButton = Button(
        sf::Vector2f(50, 50),                 
        sf::Vector2f(width - 240, height - 70),          // Button position
        "files/images/pause.png",         
        sf::Color(200, 200, 200),              
        sf::Color(150, 150, 150),              
        sf::Color(100, 100, 100)              
    );
    pauseButton.setOnClick([&]() {
        pauseOnClick(timer);
    });
    leaderboardButton = Button(
        sf::Vector2f(50, 50),                  
        sf::Vector2f(width - 180, height - 70),          // Button position
        "files/images/leaderboard.png",       
        sf::Color(200, 200, 200),             
        sf::Color(150, 150, 150),              
        sf::Color(100, 100, 100)                
    );
    LeaderboardWindow leaderboardWindow(16, 25, userData);
    leaderboardButton.setOnClick([&]() {
        leaderboardOnClick(leaderboardWindow);
    });
}

void GameWindow::handleEvent() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

            if (event.mouseButton.button == sf::Mouse::Left) {
                board.handleClick(mousePosition, false);
            } else if (event.mouseButton.button == sf::Mouse::Right) {
                board.handleClick(mousePosition, true);
                int flagCount = board.countFlag();
                counter.setCount(flagCount);
                counter.updateDisplay();
            }
        }

        resetButton.handleEvent(event, window);
        debugButton.handleEvent(event, window);
        pauseButton.handleEvent(event, window);
        leaderboardButton.handleEvent(event, window);
    }
}

void GameWindow::update() {
    int elapsedTime = timer.getElapsedTimeInSeconds();
    //std::cout << "Elapsed time: " << elapsedTime << " seconds" << std::endl;
    timerDisplay.update(elapsedTime);
    board.update();
}

void GameWindow::render() {
    window.clear(sf::Color::Blue);
    timerDisplay.draw(window);
    counter.draw(window);
    board.draw(window);
    resetButton.draw(window);
    debugButton.draw(window);
    pauseButton.draw(window);
    leaderboardButton.draw(window);
    window.display();
}

LeaderboardWindow::LeaderboardWindow(int _rowCount, int _colCount, UserData& _userData) : Window(_rowCount, _colCount, _userData) {
    this->height = (_rowCount * 16)+50;
    this->width = _colCount * 16;
    window.create(sf::VideoMode(width, height), "Leaderboard Window");
}

void LeaderboardWindow::handleEvent() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void LeaderboardWindow::update() {

}

void LeaderboardWindow::render() {
    window.clear(sf::Color::Blue);
    window.display();
}

void resetOnClick(Timer& timer, Board& board) {
    cout << "Reset Button OnClick" << endl;
    timer.reset();
    timer.start();
    board.reset();
} 

void debugOnClick(Board& board) {
    cout << "Debug Button OnClick" << endl;
    for (auto& row : board.getTiles()) {
        for (auto& tile : row) {
            if (tile.isMine()) tile.reveal();
        }
    }
}

void pauseOnClick(Timer& timer) {
    cout << "Pause Button OnClick" << endl;
    if (timer.checkPauseStatus()) timer.resume();
    else timer.pause();
}

void leaderboardOnClick(LeaderboardWindow& leaderboardWindow) {
    cout << "Leaderboard Button OnClick" << endl;
    leaderboardWindow.run();
}