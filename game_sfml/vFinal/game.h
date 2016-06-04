#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>

extern void initialiseGame();
extern void updateGame();
extern void drawGameFrame();
extern void keyPressGame(sf::Keyboard::Key keyCode);
extern void mousePressGame(sf::Mouse::Button button, int x, int y);

#endif