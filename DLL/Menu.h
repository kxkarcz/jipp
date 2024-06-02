#pragma once
#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include "API.h"
#include "Game.h"

struct HighScore {
    std::string playerName;
    int score;
};

class EXPORT_API Menu {
private:
    size_t scrollIndex;
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text title;
	sf::Font titleFont;
    sf::Text titleText;
    sf::Text startText;
    sf::Text scoresText;
    sf::Text quitText;
    sf::Text quitButtonText;
    sf::RectangleShape quitButton;
    sf::Texture menuTexture; 
    sf::Sprite menuSprite;  
    bool gameStartClicked;
    bool showScoresClicked;
    bool scoresScreenVisible;
    std::vector<HighScore> highScores;

    void loadHighScores();
    void drawHighScores();
    friend class Game;
public:
    Menu(sf::RenderWindow& window);

    void handleInput(sf::Event& event);
    void handleScoresScreenInput(sf::Event& event);
    void update();
    void draw();
    void drawScoresScreen();

    bool isStartGameClicked() const;
    bool isShowScoresClicked() const;
    void setStartGame(bool shouldStart);
    void setShowScores(bool shouldShow);
    void reset();
};
#endif // MENU_H