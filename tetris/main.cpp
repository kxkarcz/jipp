#include "Menu.h"
#include <SFML/Graphics.hpp>
#include "Game.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tetris");

    Menu menu(window);
    Game game(window);
    bool inMenu = true;

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

    return 0;
}
