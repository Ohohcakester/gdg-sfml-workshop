#include "main.h"
#include "Camera.h"
#include "Player.h"
#include "GameVariables.h"

void absToRel(float& x, float& y) {
    x -= game.camera->x - RES_X/2;
    y -= game.camera->y - game.floorHeight;
}

void relToAbs(float& x, float& y) {
    x += game.camera->x - RES_X/2;
    y += game.camera->y - game.floorHeight;
}

Camera::Camera(int x, int y) {
    this->x = x;
    this->y = y;
}

void Camera::update() {
    float p = 0.1f;

    x += p * (game.player->x - x);
    y += p * (game.player->y - y);
}