#include "main.h"
#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"
#include "Camera.h"
#include "GameVariables.h"
#include "Sfx.h"

Enemy::Enemy(int x, int y, int vx, int vy) {
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->ax = 0;
    this->ay = 0.07f;
}

void Enemy::update() {
    if (!isActive) return;

    vx += ax;
    vy += ay;
    x += vx;
    y += vy;
    
    if (vy > 0 && y + radius > game.floorHeight) {
        y = 2*(game.floorHeight-radius) - y;
        vy = -vy;
    }

    checkCollisions();
}

void Enemy::draw() {
    if (!isActive) return;

    sf::CircleShape shape;
    shape.setFillColor(sf::Color::Yellow);
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    float drawX = x, drawY = y;
    absToRel(drawX, drawY);
    shape.setPosition(drawX, drawY);
    window->draw(shape);
}

bool Enemy::isCollidingWith(Bullet& bullet) {
    if (!bullet.isActive) return false;

    float dx = x - bullet.x;
    float dy = y - bullet.y;
    float r = radius + bullet.radius;
    return dx*dx + dy*dy <= r*r;
}

bool Enemy::isCollidingWith(Player* player) {
    if (!player->isActive) return false;

    float dx = x - player->x;
    float dy = y - player->y;
    float r = radius + player->hitboxRadius;
    return dx*dx + dy*dy <= r*r;

}

void Enemy::checkCollisions() {
    for (int i=0;i<game.bullets.size();++i) {
        if (isCollidingWith(game.bullets[i])) {
            game.sfxs.push_back(new BulletPop(game.bullets[i].x, game.bullets[i].y));
            for (int j=0;j<10;++j) game.sfxs.push_back(new ExplodeParticle(x,y));

            game.bullets[i].isActive = false;
            this->isActive = false;
        }
    }
    if (isCollidingWith(game.player)) {
        game.player->isActive = false;
    }
}

