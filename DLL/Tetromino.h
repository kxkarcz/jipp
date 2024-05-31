#ifndef TETROMINO_H
#define TETROMINO_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "API.h"
#include "Board.h"

class EXPORT_API Tetromino {
public:
    Tetromino();
    void reset();
    void draw(sf::RenderWindow& window);
    bool moveDown(const Board& board);
    void moveLeft(const Board& board);
    void moveRight(const Board& board);
    void rotate(const Board& board);

    sf::Vector2i getPosition() const;
    void setPosition(sf::Vector2i pos);
    const std::vector<sf::Vector2i>& getShape() const;
    void setBlock(int index, sf::Vector2i block);
    sf::Color getColor() const;

private:
    sf::Vector2i position;
    std::vector<sf::Vector2i> shape;
    sf::Color color;
};

#endif // TETROMINO_H
