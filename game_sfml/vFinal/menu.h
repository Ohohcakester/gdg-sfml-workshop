#ifndef MENU_H_
#define MENU_H_

#include <SFML/Graphics.hpp>

extern void initialiseMenu();
extern void updateMenu();
extern void drawMenuFrame();
extern void keyPressMenu(sf::Keyboard::Key keyCode);
extern void mousePressMenu(sf::Mouse::Button button, int x, int y);

#endif