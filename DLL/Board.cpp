#include "Board.h"
#include <SFML/Graphics.hpp>
#include "Game.h"
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

void Board::draw(sf::RenderWindow& window, sf::Vector2f position) {
    // Rysowanie planszy
    sf::RectangleShape blockShape(sf::Vector2f(30.0f, 30.0f));
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (grid[y][x] != sf::Color::Black) {
                blockShape.setFillColor(grid[y][x]);
                blockShape.setPosition(x * 30.0f + position.x , y * 30.0f + position.y); // Ustawienie pozycji bloku planszy z uwzglêdnieniem przesuniêcia
                window.draw(blockShape);
            }
        }
    }

    // Rysowanie obramowania
    sf::RectangleShape borderShape(sf::Vector2f(WIDTH * 30.0f, HEIGHT * 30.0f));
    borderShape.setFillColor(sf::Color::Transparent);
    borderShape.setOutlineColor(sf::Color::White); // Ustawienie koloru obramowania na bia³y
    borderShape.setOutlineThickness(2.0f); // Ustawienie gruboœci obramowania
    borderShape.setPosition(position); // Ustawienie pozycji obramowania
    window.draw(borderShape);
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
