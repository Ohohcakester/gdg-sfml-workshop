#include "main.h"
#include "menu.h"
#include "game.h"

/// Globals
sf::RenderWindow* window;
int RES_X = 800;
int RES_Y = 600;

bool key_left = false;
bool key_right = false;

bool inGame;

void keyDown(sf::Keyboard::Key keyCode) {
    switch(keyCode) {
        case sf::Keyboard::A: key_left = true; break;
        case sf::Keyboard::D: key_right = true; break;
    }

    if (inGame) keyPressGame(keyCode);
    else keyPressMenu(keyCode);
}

void keyUp(sf::Keyboard::Key keyCode) {
    switch(keyCode) {
        case sf::Keyboard::A: key_left = false; break;
        case sf::Keyboard::D: key_right = false; break;
    }
}


void processEvent(sf::Event& event) {
    switch(event.type) {
        case sf::Event::Closed: {
            window->close();
            break;
        }
        case sf::Event::KeyPressed: {
            keyDown(event.key.code);
            break;
        }
        case sf::Event::KeyReleased: {
            keyUp(event.key.code);
            break;
        }
        case sf::Event::MouseButtonPressed: {
            if (inGame) mousePressGame(event.mouseButton.button, event.mouseButton.x, event.mouseButton.y);
            else mousePressMenu(event.mouseButton.button, event.mouseButton.x, event.mouseButton.y);
            break;
        }
    }
}

int main() {
    window = new sf::RenderWindow(sf::VideoMode(RES_X, RES_Y), "I love this Game");

    sf::Clock clock;
    float frameTime = 1/60.0f;
    float dTime = 0;

    initialiseMenu();

    while (window->isOpen()) {
        dTime += clock.getElapsedTime().asSeconds();
        clock.restart();

        // Event handling
        sf::Event event;
        while(window->pollEvent(event)) {
            processEvent(event);
        }

        // Safeguard (slowdown) to prevent game from lagging to death
        if (dTime > 5*frameTime) dTime = 5*frameTime;

        // Update game
        while (dTime > frameTime) {
            dTime -= frameTime;
            if (inGame) updateGame();
            else updateMenu();
        }

        // Draw frame
        window->clear();
        if (inGame) drawGameFrame();
        else drawMenuFrame();
        window->display();
    }

    delete window;
    return 0;
}