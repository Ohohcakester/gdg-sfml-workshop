#ifndef PLAYER_H_
#define PLAYER_H_

class Player {
public:
    bool isActive = true;
    int x, y;
    int width = 60;
    int height = 40;
    int speed = 5;
    int hitboxRadius = 10;
    float bulletSpeed = 25;
    
    Player(int x, int y): x(x), y(y) {}
    
    void update();
    void draw();
    void shoot(int targetX, int targetY);
};



#endif