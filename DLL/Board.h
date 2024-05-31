#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include "Board.h"
#include <array>
#include <vector>
#include "API.h"

class EXPORT_API Board {
public:
    static const int WIDTH = 10;
    static const int HEIGHT = 20;

    Board();

    void reset();
    void draw(sf::RenderWindow& window);
    bool isCollision(const std::vector<sf::Vector2i>& shape, sf::Vector2i position) const;
    void placeTetromino(const std::vector<sf::Vector2i>& shape, sf::Vector2i position, sf::Color color);
    int clearLines();
    bool checkGameOver(const std::vector<sf::Vector2i>& shape, sf::Vector2i position) const;

    sf::Color getBlock(int x, int y) const;
    void setBlock(int x, int y, sf::Color color);

private:
    sf::Color grid[HEIGHT][WIDTH];
};

#endif // BOARD_H