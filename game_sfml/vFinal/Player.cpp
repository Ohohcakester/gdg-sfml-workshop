#include "main.h"
#include "Player.h"
#include "Bullet.h"
#include "Camera.h"
#include "GameVariables.h"

void Player::update() {
    if (!isActive) return;

    if (key_left) x -= speed;
    if (key_right) x += speed;
}

void Player::draw() {
    if (!isActive) return;

    sf::RectangleShape shape;
    shape.setFillColor(sf::Color::Blue);
    shape.setSize(sf::Vector2f(width, height));
    shape.setOrigin(width/2, height/2);
    float drawX = x, drawY = y;
    absToRel(drawX, drawY);
    shape.setPosition(drawX, drawY);
    window->draw(shape);
}

void Player::shoot(int targetX, int targetY) {
    if (!isActive) return;

    float angle = atan2(targetY - y, targetX - x);
    game.bullets.push_back(Bullet(x,y,angle,bulletSpeed));
}