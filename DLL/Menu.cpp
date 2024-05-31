#include "Menu.h"
#include <iostream>
#include <fstream>
#include <sstream> // Dodano nag³ówek dla stringstream

Menu::Menu(sf::RenderWindow& window) : window(window), gameStartClicked(false), showScoresClicked(false), scoresScreenVisible(false), scrollIndex(0) {
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Unable to load font");
    }
    titleText.setFont(font);
    titleText.setString("Menu");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(100, 50);

    startText.setFont(font);
    startText.setString("Start Game");
    startText.setCharacterSize(30);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(100, 150);

    scoresText.setFont(font);
    scoresText.setString("High Scores");
    scoresText.setCharacterSize(30);
    scoresText.setFillColor(sf::Color::White);
    scoresText.setPosition(100, 200);

    quitText.setFont(font);
    quitText.setString("Quit");
    quitText.setCharacterSize(30);
    quitText.setFillColor(sf::Color::White);
    quitText.setPosition(100, 250);

    loadHighScores();

    quitButton.setSize(sf::Vector2f(150, 50));
    quitButton.setFillColor(sf::Color::White);
    quitButton.setPosition(250, 420);

    quitButtonText.setFont(font);
    quitButtonText.setString("Quit");
    quitButtonText.setCharacterSize(24);
    quitButtonText.setFillColor(sf::Color::Black);
    quitButtonText.setPosition(
        quitButton.getPosition().x + (quitButton.getSize().x / 2.f) - (quitButtonText.getLocalBounds().width / 2.f),
        quitButton.getPosition().y + (quitButton.getSize().y / 2.f) - (quitButtonText.getLocalBounds().height / 2.f)
    );
}

void Menu::handleInput(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (startText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            gameStartClicked = true;
        }
        else if (scoresText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            setShowScores(true); // Poka¿ wyniki
        }
        else if (quitText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            window.close();
        }
    }
}


void Menu::update() {

}

void Menu::draw() {
    if (scoresScreenVisible) {
        drawScoresScreen();
    }
    else {
        window.draw(titleText);
        window.draw(startText);
        window.draw(scoresText);
        window.draw(quitText);
    }
}

bool Menu::isStartGameClicked() const {
    return gameStartClicked;
}

bool Menu::isShowScoresClicked() const {
    return showScoresClicked;
}

void Menu::setStartGame(bool shouldStart) {
    gameStartClicked = shouldStart;
}

void Menu::setShowScores(bool shouldShow) {
    showScoresClicked = shouldShow;
    scoresScreenVisible = shouldShow; // Poka¿ nowy ekran z wynikami
}

void Menu::reset() {
    gameStartClicked = false;
    showScoresClicked = false;
    scoresScreenVisible = false;
}

void Menu::loadHighScores() {
    std::ifstream file("highscores.txt");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            HighScore hs;
            if (iss >> hs.playerName >> hs.score) {
                highScores.push_back(hs);
            }
        }
        file.close();
    }

    // Sortujemy wyniki malej¹co
    std::sort(highScores.begin(), highScores.end(), [](const HighScore& a, const HighScore& b) {
        return a.score > b.score;
        });
}

void Menu::drawHighScores() {
    sf::Text highScoresText;
    highScoresText.setFont(font);
    highScoresText.setCharacterSize(24);
    highScoresText.setFillColor(sf::Color::White);
    highScoresText.setPosition(250, 400);

    std::stringstream ss;
    ss << "High Scores:\n";
    for (size_t i = 0; i < highScores.size(); ++i) {
        ss << i + 1 << ". " << highScores[i].playerName << ": " << highScores[i].score << "\n";
    }

    highScoresText.setString(ss.str());
    window.draw(highScoresText);
}

void Menu::drawScoresScreen() {
    sf::RectangleShape background(sf::Vector2f(400, 300));
    background.setFillColor(sf::Color(0, 0, 0, 200));
    background.setPosition(200, 100);
    window.draw(background);

    sf::Text title("High Scores", font, 36);
    title.setFillColor(sf::Color::White);
    title.setPosition(250, 120);
    window.draw(title);

    // Okreœl widoczn¹ listê wyników w zale¿noœci od paska przewijania
    size_t startIndex = scrollIndex;
    size_t endIndex = std::min(scrollIndex + 5, highScores.size());

    // Wyœwietl widoczn¹ listê wyników
    for (size_t i = startIndex; i < endIndex; ++i) {
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(250, 180 + (i - startIndex) * 30);
        scoreText.setString(std::to_string(i + 1) + ". " + highScores[i].playerName + ": " + std::to_string(highScores[i].score));
        window.draw(scoreText);
    }

    // Oblicz proporcje paska przewijania
    float scrollBarHeight = 300.0f * 5 / highScores.size();
    float scrollBarPosition = 100.0f + 300.0f * scrollIndex / highScores.size();

    // Narysuj pasek przewijania
    sf::RectangleShape scrollBar(sf::Vector2f(10, scrollBarHeight));
    scrollBar.setFillColor(sf::Color::White);
    scrollBar.setPosition(610, scrollBarPosition);
    window.draw(scrollBar);

    window.draw(quitButton);
    window.draw(quitButtonText);
}

void Menu::handleScoresScreenInput(sf::Event& event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            if (event.mouseWheelScroll.delta > 0 && scrollIndex > 0) {
                --scrollIndex; // Przewij w górê, jeœli mo¿liwe
            }
            else if (event.mouseWheelScroll.delta < 0 && scrollIndex + 5 < highScores.size()) {
                ++scrollIndex; // Przewij w dó³, jeœli mo¿liwe
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        sf::FloatRect quitButtonBounds = quitButton.getGlobalBounds();

        if (quitButtonBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            setShowScores(false);
        }
    }
}



