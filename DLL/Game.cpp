#include "Game.h"
#include "Menu.h"
#include "Exceptions.h"
#include <fstream>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <iostream>

Game::Game(sf::RenderWindow& window) : window(window), score(0), boardPosition(300.0f, 0.0f) {
    if (!font.loadFromFile("arial.ttf")) {
		throw EXCEPTION_CODE::FontNotFound;
    }
	if (!gameOverFont.loadFromFile("tetris.ttf")) {
        throw EXCEPTION_CODE::FontNotFound;
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
                showGameOverScreen(); // Wywo�anie ekranu gratulacyjnego po przegranej grze
                reset(); // Zresetowanie gry po przegranej
            }
        }
        clock.restart();
    }
    scoreText.setString("Score: " + std::to_string(score));
	scoreText.setPosition(window.getSize().x - scoreText.getLocalBounds().width - 600, 10);
	scoreText.setCharacterSize(40);
}

void Game::draw() {

	board.draw(window, boardPosition);
    tetromino.draw(window);
    //scoreText.setPosition(window.getSize().x - scoreText.getLocalBounds().width-500, 0);

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



void Game::saveScore(const std::string& playerName, int score) {
    std::ofstream scoreFile("highscores.dat", std::ios::binary | std::ios::app);
    if (!scoreFile.is_open()) {
        throw EXCEPTION_CODE::FileForSavingNotFound;
    }

    // Write player name length and name
    size_t nameLength = playerName.size();
    scoreFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    scoreFile.write(playerName.c_str(), nameLength);

    // Write score
    scoreFile.write(reinterpret_cast<const char*>(&score), sizeof(score));

    scoreFile.close();
}



void Game::showGameOverScreen() {
    sf::Text gameOverText;
    gameOverText.setFont(gameOverFont);
    gameOverText.setCharacterSize(72);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("game over");
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

    sf::RectangleShape saveButtonRect(sf::Vector2f(160, 70)); // Zmieniamy wysokość prostokąta na 70
    saveButtonRect.setFillColor(sf::Color::White);
    saveButtonRect.setOutlineColor(sf::Color::Black);
    saveButtonRect.setOutlineThickness(2);
    saveButtonRect.setPosition((window.getSize().x - saveButtonRect.getSize().x) / 2, 500); // Wyrównujemy prostokąt do środka ekranu

    sf::Text saveButtonText;
    saveButtonText.setFont(font);
    saveButtonText.setCharacterSize(24);
    saveButtonText.setFillColor(sf::Color::Black);
    saveButtonText.setString("Save");

    // Ustawiamy pozycję tekstu na środku prostokąta
    saveButtonText.setPosition(saveButtonRect.getPosition().x + (saveButtonRect.getSize().x - saveButtonText.getLocalBounds().width) / 2,
        saveButtonRect.getPosition().y + (saveButtonRect.getSize().y - saveButtonText.getLocalBounds().height) / 2);

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
                        saveScore(playerName, score);
                        menu.loadHighScores();
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

    menu.reset();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (menu.scoresScreenVisible) {
                menu.handleScoresScreenInput(event);
            }
            else {
                menu.handleInput(event);
                if (menu.isStartGameClicked()) {
                    reset();
                    return;
                }
                if (menu.isShowScoresClicked()) {
                    menu.setShowScores(true);
                }
            }
        }

        window.clear();
        menu.draw();
        window.display();
    }
}

