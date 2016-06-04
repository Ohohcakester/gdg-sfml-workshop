#include "main.h"

void drawRectangle(int x1, int y1, int x2, int y2, sf::Color color) {
    sf::RectangleShape shape;
    shape.setFillColor(color);
    shape.setPosition(x1,y1);
    shape.setSize(sf::Vector2f(x2-x1, y2-y1));
    window->draw(shape);
}