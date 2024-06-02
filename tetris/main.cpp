#include "Menu.h"
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Exceptions.h"

int main() {
    try {
        sf::RenderWindow window(sf::VideoMode(800, 600), "Tetris"); //Tworzymy okno o wymiarach 800x600 i tytule "Tetris"
        sf::Image icon;
        if (!icon.loadFromFile("logo.png")) {
            return -1;
        }
        Menu menu(window);
        Game game(window);
        bool inMenu = true;
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (inMenu) {
                    menu.handleInput(event);
                    if (menu.isStartGameClicked()) {
                        inMenu = false;
                        game.reset();
                    }
                    else if (menu.isShowScoresClicked()) {
                        menu.handleScoresScreenInput(event);
                    }
                }
                else {
                    game.handleInput(event);
                }
            }

            window.clear(); // Clear the window once at the beginning of the frame

            if (inMenu) {
                menu.update(); // Update the menu state
                menu.draw();   // Draw the menu
            }
            else {
                game.update(); // Update the game state
                game.draw();   // Draw the game
            }

            window.display(); // Display the window once at the end of the frame
        }
    }
    catch (EXCEPTION_CODE error) {
		exception_message(error);
        return -1;
    }
    return 0;
}
