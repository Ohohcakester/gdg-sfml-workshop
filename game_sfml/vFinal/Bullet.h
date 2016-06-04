#ifndef BULLET_H_
#define BULLET_H_

class Bullet {
public:
    bool isActive = true;
    float x, y;
    float vx, vy;
    int radius = 5;
    
    Bullet(int x, int y, float angle, float speed);
    void update();
    void draw();
};

#endif