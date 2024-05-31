#include "Game.h"
#include "Menu.h"
#include <fstream>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <iostream>

Game::Game(sf::RenderWindow& window) : window(window), score(0) {
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Unable to load font");
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    reset();
}

void Game::reset() {
    board.reset();
    tetromino.reset();
    score = 0;
}

void Game::update() {
    if (clock.getElapsedTime().asSeconds() >= 1.0f) {
        if (!tetromino.moveDown(board)) {
            board.placeTetromino(tetromino.getShape(), tetromino.getPosition(), tetromino.getColor());
            score += board.clearLines();
            tetromino.reset();
            if (board.checkGameOver(tetromino.getShape(), tetromino.getPosition())) {
                showGameOverScreen(); // Wywo³anie ekranu gratulacyjnego po przegranej grze
                reset(); // Zresetowanie gry po przegranej
            }
        }
        clock.restart();
    }
    scoreText.setString("Score: " + std::to_string(score));
}

void Game::draw() {
    board.draw(window);
    tetromino.draw(window);
    window.draw(scoreText);
}

void Game::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left) {
            tetromino.moveLeft(board);
        }
        else if (event.key.code == sf::Keyboard::Right) {
            tetromino.moveRight(board);
        }
        else if (event.key.code == sf::Keyboard::Down) {
            tetromino.moveDown(board);
        }
        else if (event.key.code == sf::Keyboard::Up) {
            tetromino.rotate(board);
        }
    }
}


void Game::saveGame(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for saving");
    }
    file << score << "\n";
    file << tetromino.getPosition().x << " " << tetromino.getPosition().y << "\n";
    for (const auto& block : tetromino.getShape()) {
        file << block.x << " " << block.y << "\n";
    }
    for (int y = 0; y < Board::HEIGHT; ++y) {
        for (int x = 0; x < Board::WIDTH; ++x) {
            file << board.getBlock(x, y).toInteger() << " ";
        }
        file << "\n";
    }
    file.close();
}

void Game::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for loading");
    }
    file >> score;
    int x, y;
    file >> x >> y;
    tetromino.setPosition(sf::Vector2i(x, y));
    for (int i = 0; i < 4; ++i) {
        file >> x >> y;
        tetromino.setBlock(i, sf::Vector2i(x, y));
    }
    for (int y = 0; y < Board::HEIGHT; ++y) {
        for (int x = 0; x < Board::WIDTH; ++x) {
            sf::Uint32 color;
            file >> color;
            board.setBlock(x, y, sf::Color(color));
        }
    }
    file.close();
}
void Game::saveScore(const std::string& playerName, int score) {
    std::ofstream scoreFile("highscores.txt", std::ios::app);
    if (!scoreFile.is_open()) {
        throw std::runtime_error("Unable to open highscores file for saving");
    }
    scoreFile << playerName << " " << score << "\n";
    scoreFile.close();
}


void Game::showGameOverScreen() {
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(72);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("Game Over!");
    gameOverText.setPosition((window.getSize().x - gameOverText.getLocalBounds().width) / 2, 100);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(36);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setString("Your score: " + std::to_string(score));
    scoreText.setPosition((window.getSize().x - scoreText.getLocalBounds().width) / 2, 250);

    sf::Text enterNameText;
    enterNameText.setFont(font);
    enterNameText.setCharacterSize(24);
    enterNameText.setFillColor(sf::Color::White);
    enterNameText.setString("Enter your name:");
    enterNameText.setPosition(50, 350);

    sf::Text playerNameText;
    playerNameText.setFont(font);
    playerNameText.setCharacterSize(24);
    playerNameText.setFillColor(sf::Color::White);
    playerNameText.setPosition(enterNameText.getPosition().x + enterNameText.getLocalBounds().width + 20, enterNameText.getPosition().y);

    sf::RectangleShape saveButtonRect(sf::Vector2f(100, 40));
    saveButtonRect.setFillColor(sf::Color::White);
    saveButtonRect.setOutlineColor(sf::Color::Black);
    saveButtonRect.setOutlineThickness(2);
    saveButtonRect.setPosition((window.getSize().x - saveButtonRect.getSize().x) / 2, 500);

    sf::Text saveButtonText;
    saveButtonText.setFont(font);
    saveButtonText.setCharacterSize(24);
    saveButtonText.setFillColor(sf::Color::Black);
    saveButtonText.setString("Save");
    saveButtonText.setPosition(saveButtonRect.getPosition().x + (saveButtonRect.getSize().x - saveButtonText.getLocalBounds().width) / 2, saveButtonRect.getPosition().y + (saveButtonRect.getSize().y - saveButtonText.getLocalBounds().height) / 2);

    std::string playerName;
    bool nameEntered = false;
    Menu menu(window);

    while (window.isOpen() && !nameEntered) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == 8 && !playerName.empty()) {
                        playerName.pop_back();
                    }
                    else if (event.text.unicode != 8) {
                        playerName += static_cast<char>(event.text.unicode);
                    }
                    playerNameText.setString(playerName);
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    if (saveButtonRect.getGlobalBounds().contains(mousePos)) {
                        nameEntered = true;
						std::cout << score << std::endl;
                        saveScore(playerName, score);
						menu.loadHighScores();
                        menu.reset();
                        reset(); // Zresetuj grê po zapisaniu wyniku
                    }
                }
            }
        }

        window.clear();
        window.draw(gameOverText);
        window.draw(scoreText);
        window.draw(enterNameText);
        window.draw(playerNameText);
        window.draw(saveButtonRect);
        window.draw(saveButtonText);
        window.display();
    }

    // Wyœwietlanie menu g³ównego po zapisaniu wyniku
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (menu.isShowScoresClicked()) {
                menu.handleScoresScreenInput(event);
            }
            else {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                menu.handleInput(event);
            }
        }

        window.clear();
        menu.draw();
        window.display();
    }
}