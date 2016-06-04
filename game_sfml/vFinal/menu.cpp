#include "main.h"
#include "game.h"
#include "utility.h"
#include "MenuVariables.h"

void initialiseMenu() {
    inGame = false;
    menu.currentScreen = menuscreen_main;
}


void updateMenu() {

}


void drawMenuMain() {
    int highlightHalfWidth = 210;
    int highlightHalfHeight = 40;
    int buttonHalfWidth = 200;
    int buttonHalfHeight = 30;
    int buttonGap = 100 + 2*buttonHalfHeight;
    int nButtons = 2;
    
    int baseY = RES_Y/2 - (nButtons-1)*buttonGap/2;
    for (int i=0;i<2;++i) {
        int x = RES_X/2;
        int y = baseY + i*buttonGap;
        
        if (menu.menuMainOption == i) {
            drawRectangle(x-highlightHalfWidth, y-highlightHalfHeight, x+highlightHalfWidth, y+highlightHalfHeight, sf::Color::Yellow);
        }
        
        drawRectangle(x-buttonHalfWidth, y-buttonHalfHeight, x+buttonHalfWidth, y+buttonHalfHeight, sf::Color::Blue);
    }
}

void drawMenuOptions() {
    int highlightHalfSize = 40;
    int buttonHalfSize = 30;
    
    int buttonGap = 100;
    int nButtons = 6;
    
    int baseX = RES_X/2 - (nButtons-1)*buttonGap/2;
    for (int i=0;i<6;++i) {
        int x = baseX + i*buttonGap;
        int y = RES_Y/2;
        
        if (menu.difficultyLevel == i+1) {
            drawRectangle(x-highlightHalfSize, y-highlightHalfSize, x+highlightHalfSize, y+highlightHalfSize, sf::Color::Yellow);
        }
        
        drawRectangle(x-buttonHalfSize, y-buttonHalfSize, x+buttonHalfSize, y+buttonHalfSize, sf::Color::Blue);
    }
}

void drawMenuFrame() {
    window->clear(sf::Color(64,64,127));

    switch (menu.currentScreen) {
        case menuscreen_main: drawMenuMain(); break;
        case menuscreen_options: drawMenuOptions(); break;
    }
}


void keypressMenuMain(sf::Keyboard::Key keyCode) {
    switch (keyCode) {
        case sf::Keyboard::Space: {
            switch(menu.menuMainOption) {
            case menumain_startgame:
                initialiseGame();
                break;
            case menumain_options:
                menu.currentScreen = menuscreen_options;
                break;
            }
            break;
        }
        case sf::Keyboard::W: {
            --menu.menuMainOption;
            if (menu.menuMainOption < 0) menu.menuMainOption = 1;
            break;
        }
        case sf::Keyboard::S: {
            ++menu.menuMainOption;
            if (menu.menuMainOption > 1) menu.menuMainOption = 0;
            break;
        }
    }
}

void keypressMenuOptions(sf::Keyboard::Key keyCode) {
    switch (keyCode) {
        case sf::Keyboard::A: {
            --menu.difficultyLevel;
            if (menu.difficultyLevel < 1) menu.difficultyLevel = 6;
            break;
        }
        case sf::Keyboard::D: {
            ++menu.difficultyLevel;
            if (menu.difficultyLevel > 6) menu.difficultyLevel = 1;
            break;
        }
        case sf::Keyboard::Space:
        case sf::Keyboard::Escape: {
            menu.currentScreen = menuscreen_main;
            break;
        }
    }
}

void keyPressMenu(sf::Keyboard::Key keyCode) {
    switch (menu.currentScreen) {
        case menuscreen_main: keypressMenuMain(keyCode); break;
        case menuscreen_options: keypressMenuOptions(keyCode); break;
    }
}



void mousePressMenu(sf::Mouse::Button button, int x, int y) {
}