#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Tetromino.h"
#include "API.h"
#include <string>
#include <vector>
#include <utility>

class EXPORT_API Game {
public:
    Game(sf::RenderWindow& window);

    void update();
    void draw();
    void handleInput(sf::Event& event);

    void loadGame(const std::string& filename);
    void saveScore(const std::string& playerName, int score); // Deklaracja funkcji saveScore

    void showGameOverScreen(); // Deklaracja funkcji showGameOverScreen

    void reset();
private:
    sf::Vector2f boardPosition;
    int linesCleared;
    sf::RenderWindow& window;
    Board board;
    Tetromino tetromino;
    int score;
    bool inMenu;
    sf::Font font;
	sf::Font gameOverFont;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text playerNameText;
    sf::Clock clock;
    bool gameOver;
    bool enterName;
    std::string playerName;
    std::vector<std::pair<std::string, int>> highScores;

    sf::Text endGameText;  // Add these declarations
    sf::Text playerNamePromptText;
    sf::Text playerNameDisplayText;
	friend class Board;
};

#endif // GAME_H