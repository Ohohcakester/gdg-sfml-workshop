#ifndef ENEMY_H_
#define ENEMY_H_

class Bullet;
class Player;

class Enemy {
public:
    bool isActive = true;
    float x, y, vx, vy, ax, ay;
    int radius = 35;
    
    Enemy(int x, int y, int vx, int vy);
    void update();
    void draw();
    bool isCollidingWith(Bullet& bullet);
    bool isCollidingWith(Player* player);
    void checkCollisions();
};

#endif