#include "main.h"
#include "Sfx.h"
#include "Camera.h"

Sfx::Sfx(float x, float y, float vx, float vy, float ax, float ay) {
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->ax = ax;
    this->ay = ay;
    this->animframe = 0;
}

void Sfx::sfxUpdate() {
    if (!isActive) return;

    vx += ax;
    vy += ay;
    x += vx;
    y += vy;
    animframe++;
}

void BulletPop::update() {
    if (!isActive) return;

    sfxUpdate();
    if (animframe >= duration) isActive = false;
}

void BulletPop::draw() {
    if (!isActive) return;
    float radius = 1 + 60*animframe/duration;

    sf::CircleShape shape;
    shape.setFillColor(sf::Color::Transparent);
    shape.setRadius(radius);
    shape.setOutlineThickness(3);
    shape.setOutlineColor(sf::Color::Cyan);
    shape.setOrigin(radius, radius);
    float drawX = x, drawY = y;
    absToRel(drawX, drawY);
    shape.setPosition(drawX, drawY);
    window->draw(shape);
}


ExplodeParticle::ExplodeParticle(float x, float y) : Sfx(x,y) {
    vx = (rand()%51 - 25)/5.0f;
    vy = (rand()%51 - 25)/5.0f;
    ay = 0.07f;
    duration = 30 + rand()%20;
}

void ExplodeParticle::update() {
    if (!isActive) return;

    sfxUpdate();
    if (animframe >= duration) isActive = false;
}

void ExplodeParticle::draw() {
    if (!isActive) return;

    int radius = 5;
    sf::CircleShape shape;
    shape.setFillColor(sf::Color::Yellow);
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    float drawX = x, drawY = y;
    absToRel(drawX, drawY);
    shape.setPosition(drawX, drawY);
    window->draw(shape);
}