#include "Board.h"
#include <SFML/Graphics.hpp>

Board::Board() {
    reset();
}

void Board::reset() {
    for (auto& row : grid) {
        for (auto& block : row) {
            block = sf::Color::Black;
        }
    }
}

void Board::draw(sf::RenderWindow& window) {
    sf::RectangleShape blockShape(sf::Vector2f(30.0f, 30.0f));
    // Rysowanie czarnej planszy
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            blockShape.setFillColor(sf::Color::Black); // Ustawienie koloru czarnej kratki
            blockShape.setPosition(x * 30.0f, y * 30.0f);
            window.draw(blockShape);
        }
    }
    // Rysowanie szarych linii - kratki
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (grid[y][x] == sf::Color::Black) {
                blockShape.setFillColor(sf::Color(150, 150, 150)); // Ustawienie koloru szarej linii - kratki
                blockShape.setPosition(x * 30.0f, y * 30.0f);
                window.draw(blockShape);
            }
        }
    }
}



bool Board::isCollision(const std::vector<sf::Vector2i>& shape, sf::Vector2i position) const {
    for (const auto& block : shape) {
        int x = block.x + position.x;
        int y = block.y + position.y;
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT || grid[y][x] != sf::Color::Black) {
            return true;
        }
    }
    return false;
}

void Board::placeTetromino(const std::vector<sf::Vector2i>& shape, sf::Vector2i position, sf::Color color) {
    for (const auto& block : shape) {
        int x = block.x + position.x;
        int y = block.y + position.y;
        grid[y][x] = color;
    }
}

int Board::clearLines() {
    int linesCleared = 0;
    for (int y = 0; y < HEIGHT; ++y) {
        bool fullLine = true;
        for (int x = 0; x < WIDTH; ++x) {
            if (grid[y][x] == sf::Color::Black) {
                fullLine = false;
                break;
            }
        }
        if (fullLine) {
            ++linesCleared;
            for (int yy = y; yy > 0; --yy) {
                for (int x = 0; x < WIDTH; ++x) {
                    grid[yy][x] = grid[yy - 1][x];
                }
            }
            for (int x = 0; x < WIDTH; ++x) {
                grid[0][x] = sf::Color::Black;
            }
        }
    }
    return linesCleared;
}

bool Board::checkGameOver(const std::vector<sf::Vector2i>& shape, sf::Vector2i position) const {
    return isCollision(shape, position);
}

sf::Color Board::getBlock(int x, int y) const {
    return grid[y][x];
}

void Board::setBlock(int x, int y, sf::Color color) {
    grid[y][x] = color;
}
