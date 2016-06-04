#include "main.h"
#include "menu.h"
#include "GameVariables.h"
#include "MenuVariables.h"
#include "Enemy.h"
#include "Player.h"
#include "Sfx.h"
#include "Bullet.h"
#include "Camera.h"

void drawFloor() {
    sf::RectangleShape shape;
    shape.setFillColor(sf::Color::Green);
    float drawX = 0, drawY = game.floorHeight;
    absToRel(drawX, drawY);
    shape.setPosition(0, drawY);
    shape.setSize(sf::Vector2f(RES_X, RES_Y-game.floorHeight));
    window->draw(shape);
}

void spawnEnemy() {
    int x = game.player->x + RES_X + 50;
    int y = 100 + rand()%200;
    int vx = - 1 - rand()%3;
    int vy = rand()%5 - 2;
    
    game.enemies.push_back(Enemy(x,y,vx,vy));
}

void initialiseGame() {
    inGame = true;

    if (game.player != nullptr) delete game.player;
    if (game.camera != nullptr) delete game.camera;
    game.player = new Player(RES_X/2, game.floorHeight - 20);
    game.camera = new Camera(game.player->x, game.player->y);
    game.bullets.clear();
    game.enemies.clear();

    for (int i=0;i<game.sfxs.size();++i) {
        delete game.sfxs[i];
    }
    game.sfxs.clear();

    game.frame = 0;
    
    switch(menu.difficultyLevel) {
        case 1: game.enemySpawnInterval = 80; break;
        case 2: game.enemySpawnInterval = 60; break;
        case 3: game.enemySpawnInterval = 50; break;
        case 4: game.enemySpawnInterval = 40; break;
        case 5: game.enemySpawnInterval = 25; break;
        case 6: game.enemySpawnInterval = 10; break;
    }
}

void updateGame() {
    game.player->update();
    game.camera->update();

    int numBullets = 0;
    for (int i=0;i<game.bullets.size();++i) {
        game.bullets[i].update();
        if (game.bullets[i].isActive) numBullets++;
    }
    if (numBullets * 2 < game.bullets.size()) {
        // Cleanup array.
        std::vector<Bullet> newBullets;
        newBullets.reserve(numBullets);
        for (int i=0;i<game.bullets.size();++i) {
            if (game.bullets[i].isActive) {
                newBullets.push_back(game.bullets[i]);
            }
        }
        game.bullets.swap(newBullets);
    }

    int numEnemies = 0;
    for (int i=0;i<game.enemies.size();++i) {
        game.enemies[i].update();
        if (game.enemies[i].isActive) numEnemies++;
    }
    if (numEnemies * 2 < game.enemies.size()) {
        // Cleanup array.
        std::vector<Enemy> newEnemies;
        newEnemies.reserve(numEnemies);
        for (int i=0;i<game.enemies.size();++i) {
            if (game.enemies[i].isActive) {
                newEnemies.push_back(game.enemies[i]);
            }
        }
        game.enemies.swap(newEnemies);
    }

    int numSfxs = 0;
    for (int i=0;i<game.sfxs.size();++i) {
        game.sfxs[i]->update();
        if (game.sfxs[i]->isActive) numSfxs++;
    }
    if (numSfxs * 2 < game.sfxs.size()) {
        // Cleanup array.
        std::vector<Sfx*> newSfxs;
        newSfxs.reserve(numSfxs);
        for (int i=0;i<game.sfxs.size();++i) {
            if (game.sfxs[i]->isActive) {
                newSfxs.push_back(game.sfxs[i]);
            } else {
                delete game.sfxs[i];
            }
        }
        game.sfxs.swap(newSfxs);
    }

    if (game.frame%game.enemySpawnInterval == 0) {
        spawnEnemy();
    }
    game.frame++;
}

void drawGameFrame() {
    drawFloor();

    game.player->draw();
    for (int i=0;i<game.bullets.size();++i) {
        game.bullets[i].draw();
    }
    for (int i=0;i<game.enemies.size();++i) {
        game.enemies[i].draw();
    }
    for (int i=0;i<game.sfxs.size();++i) {
        game.sfxs[i]->draw();
    }
}

void keyPressGame(sf::Keyboard::Key keyCode) {
    switch(keyCode) {
        case sf::Keyboard::R: {
            initialiseGame();
            break;
        }
        case sf::Keyboard::Escape: {
            initialiseMenu();
            break;
        }
    }
}

void mousePressGame(sf::Mouse::Button button, int x, int y) {
    float absX = x, absY = y;
    relToAbs(absX, absY);

    switch(button) {
        case sf::Mouse::Left: {
            game.player->shoot(absX, absY);
            break;
        }
    }
}

