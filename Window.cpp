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
    board = Board(_rowCount, _colCount, 50);
}

void GameWindow::handleEvent() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
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
    board.draw(window);
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