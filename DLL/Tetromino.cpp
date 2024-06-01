#include "Tetromino.h"
#include "Board.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <cstdlib>
#include <random>
#include "Game.h"

Tetromino::Tetromino() {
    reset();
}

void Tetromino::reset() {
    std::array<std::array<sf::Vector2i, 4>, 7> shapes = {
        std::array<sf::Vector2i, 4>{sf::Vector2i(0, 0), sf::Vector2i(1, 0), sf::Vector2i(0, 1), sf::Vector2i(1, 1)}, // O
        std::array<sf::Vector2i, 4>{sf::Vector2i(0, 0), sf::Vector2i(-1, 0), sf::Vector2i(1, 0), sf::Vector2i(2, 0)}, // I
        std::array<sf::Vector2i, 4>{sf::Vector2i(0, 0), sf::Vector2i(1, 0), sf::Vector2i(-1, 0), sf::Vector2i(-1, 1)}, // J
        std::array<sf::Vector2i, 4>{sf::Vector2i(0, 0), sf::Vector2i(-1, 0), sf::Vector2i(1, 0), sf::Vector2i(1, 1)}, // L
        std::array<sf::Vector2i, 4>{sf::Vector2i(0, 0), sf::Vector2i(1, 0), sf::Vector2i(0, 1), sf::Vector2i(-1, 1)}, // S
        std::array<sf::Vector2i, 4>{sf::Vector2i(0, 0), sf::Vector2i(-1, 0), sf::Vector2i(0, 1), sf::Vector2i(1, 1)}, // Z
        std::array<sf::Vector2i, 4>{sf::Vector2i(0, 0), sf::Vector2i(-1, 0), sf::Vector2i(1, 0), sf::Vector2i(0, 1)}  // T
    };
    static std::array<sf::Color, 7> colors = {
        sf::Color::Yellow, sf::Color::Cyan, sf::Color::Blue,
        sf::Color::Red, sf::Color::Magenta, sf::Color::Green, sf::Color::White
    };

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, shapes.size() - 1);

    int shapeIndex = dis(gen);
    shape.assign(shapes[shapeIndex].begin(), shapes[shapeIndex].end());
    color = colors[shapeIndex];
    position = sf::Vector2i(Board::WIDTH / 2, 0);
}

void Tetromino::draw(sf::RenderWindow& window) {
    sf::RectangleShape blockShape(sf::Vector2f(30.0f, 30.0f));
    blockShape.setFillColor(color);
    for (const auto& block : shape) {
        blockShape.setPosition((position.x + block.x+10.0f) * 30.0f, (position.y + block.y) * 30.0f);
        window.draw(blockShape);
    }
}

bool Tetromino::moveDown(const Board& board) {
    sf::Vector2i newPosition = position;
    newPosition.y += 1;
    if (!board.isCollision(shape, newPosition)) {
        position = newPosition;
        return true;
    }
    return false;
}

void Tetromino::moveLeft(const Board& board) {
    sf::Vector2i newPosition = position;
    newPosition.x -= 1;
    if (!board.isCollision(shape, newPosition)) {
        position = newPosition;
    }
}

void Tetromino::moveRight(const Board& board) {
    sf::Vector2i newPosition = position;
    newPosition.x += 1;
    if (!board.isCollision(shape, newPosition)) {
        position = newPosition;
    }
}

void Tetromino::rotate(const Board& board) {
    std::vector<sf::Vector2i> newShape(shape.begin(), shape.end());
    for (size_t i = 0; i < newShape.size(); ++i) {
        newShape[i] = sf::Vector2i(-newShape[i].y, newShape[i].x);
    }
    if (!board.isCollision(newShape, position)) {
        shape.assign(newShape.begin(), newShape.end());
    }
}

sf::Vector2i Tetromino::getPosition() const {
    return position;
}

void Tetromino::setPosition(sf::Vector2i pos) {
    position = pos;
}

const std::vector<sf::Vector2i>& Tetromino::getShape() const {
    return shape;
}

void Tetromino::setBlock(int index, sf::Vector2i block) {
    if (index >= 0 && index < shape.size()) {
        shape[index] = block;
    }
}

sf::Color Tetromino::getColor() const {
    return color;
}