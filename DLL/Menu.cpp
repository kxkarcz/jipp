#include "Menu.h"
#include <iostream>
#include <fstream>
#include <sstream> // Dodano nag³ówek dla stringstream
#include "Exceptions.h"

Menu::Menu(sf::RenderWindow& window) : window(window), gameStartClicked(false), showScoresClicked(false), scoresScreenVisible(false), scrollIndex(0) {
    if (!font.loadFromFile("arial.ttf")) {
        throw EXCEPTION_CODE::FontNotFound;
    }
    if (!titleFont.loadFromFile("tetris.ttf")) {
        throw EXCEPTION_CODE::FontNotFound;
    }
    if (!menuTexture.loadFromFile("menu.png")) {
        throw EXCEPTION_CODE::ImageNotFound;
    }
	menuSprite.setTexture(menuTexture);

    title.setFont(titleFont);
    title.setString("tetris");
    title.setCharacterSize(80);
    title.setFillColor(sf::Color::Green);
    title.setPosition(210, 10);

    titleText.setFont(font);
    titleText.setString("MENU");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition(300, 120);

    startText.setFont(font);
    startText.setString("Start Game");
    startText.setCharacterSize(30);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(280, 230);

    scoresText.setFont(font);
    scoresText.setString("High Scores");
    scoresText.setCharacterSize(30);
    scoresText.setFillColor(sf::Color::White);
    scoresText.setPosition(280, 290);

    quitText.setFont(font);
    quitText.setString("Quit");
    quitText.setCharacterSize(30);
    quitText.setFillColor(sf::Color::White);
    quitText.setPosition(280, 350);

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

        // SprawdŸ czy klikniêto na przycisk "High Scores"
        if (scoresText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            setShowScores(true); // Poka¿ wyniki
        }
        else { // Jeœli nie klikniêto na "High Scores", przeka¿ zdarzenie do obs³ugi standardowych akcji dla menu
            if (startText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                setStartGame(true); // Ustaw flagê na rozpoczêcie gry
            }
            else if (quitText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                window.close();
            }
        }
    }
}

void Menu::update() {
    // Tutaj mo¿esz dodaæ aktualizacje stanu menu, jeœli s¹ potrzebne
}

void Menu::draw() {
    window.clear();
    window.draw(menuSprite); // Narysuj t³o menu

    if (scoresScreenVisible) {
        drawScoresScreen();
    }
    else {
        // Calculate the bounding box for all the menu options
        float minX = std::min({ startText.getPosition().x, scoresText.getPosition().x, quitText.getPosition().x });
        float minY = std::min({ startText.getPosition().y, scoresText.getPosition().y, quitText.getPosition().y });
        float maxX = std::max({ startText.getPosition().x + startText.getLocalBounds().width,
                                scoresText.getPosition().x + scoresText.getLocalBounds().width,
                                quitText.getPosition().x + quitText.getLocalBounds().width });
        float maxY = std::max({ startText.getPosition().y + startText.getLocalBounds().height,
                                scoresText.getPosition().y + scoresText.getLocalBounds().height,
                                quitText.getPosition().y + quitText.getLocalBounds().height });

        // Create background rectangle
        sf::RectangleShape menuBackground(sf::Vector2f(maxX - minX + 20, maxY - minY + 40));
        menuBackground.setFillColor(sf::Color::Black);
        menuBackground.setPosition(minX - 10, minY - 20);

        // Draw background rectangle
        window.draw(menuBackground);

        // Draw texts
        window.draw(title);
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
    scrollIndex = 0; // Resetuj tak¿e indeks przewijania ekranu wyników
}

void Menu::loadHighScores() {
    std::ifstream file("highscores.dat", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open highscores file." << std::endl;
        return;
    }

    while (true) {
        HighScore hs;

        // Read player name length
        size_t nameLength;
        if (!file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength))) {
            break;  // Break if unable to read name length (end of file)
        }

        // Allocate memory for player name
        char* nameBuffer = new (std::nothrow) char[nameLength + 1];
        if (!nameBuffer) {
            // Handle memory allocation failure
			throw EXCEPTION_CODE::MemoryAllocationFailed;
        }

        // Read player name
        if (!file.read(nameBuffer, nameLength)) {
            // Handle read failure
			throw EXCEPTION_CODE::FailedToReadPlayerName;
 
            //delete[] nameBuffer;  // Deallocate memory before exiting
            //break;
        }
        nameBuffer[nameLength] = '\0';
        hs.playerName = std::string(nameBuffer);
        delete[] nameBuffer;
        // Read score
        if (!file.read(reinterpret_cast<char*>(&hs.score), sizeof(hs.score))) {
            // Handle read failure
            throw EXCEPTION_CODE::FailedToReadScore;
            //std::cerr << "Failed to read score." << std::endl;
            //break;
        }

        // Push the HighScore object into the vector
        highScores.push_back(hs);
    }

    file.close();

    // Sortujemy wyniki malej¹co
    std::sort(highScores.begin(), highScores.end(), [](const HighScore& a, const HighScore& b) {
        return a.score > b.score;
        });
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
