#include "main.h"
#include "Bullet.h"
#include "Camera.h"

Bullet::Bullet(int x, int y, float angle, float speed) {
    this->x = x;
    this->y = y;
    vx = speed*cos(angle);
    vy = speed*sin(angle);
}

void Bullet::update() {
    if (!isActive) return;

    x += vx;
    y += vy;
}

void Bullet::draw() {
    if (!isActive) return;

    sf::CircleShape shape;
    shape.setFillColor(sf::Color::Cyan);
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    float drawX = x, drawY = y;
    absToRel(drawX, drawY);
    shape.setPosition(drawX, drawY);
    window->draw(shape);
}
