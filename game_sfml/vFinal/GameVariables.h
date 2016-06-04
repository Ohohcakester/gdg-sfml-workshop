#ifndef GAME_VARIABLES_H_
#define GAME_VARIABLES_H_

#include <vector>

class Player;
class Bullet;
class Enemy;
class Camera;
class Sfx;

class GameVariables {
public:
    Player* player;
    Camera* camera;
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    std::vector<Sfx*> sfxs;

    int frame;
    int enemySpawnInterval;

    int floorHeight = 500;
};

extern GameVariables game;


#endif