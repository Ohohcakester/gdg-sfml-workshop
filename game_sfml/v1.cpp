#include <SFML/Graphics.hpp>
#include <iostream>

/// Globals
sf::RenderWindow* window;
int RES_X = 800;
int RES_Y = 600;

void initialiseGame() {
}

void updateGame() {
}

void drawGameFrame() {
}

void processEvent(sf::Event& event) {
    switch(event.type) {
        case sf::Event::Closed: {
            window->close();
            break;
        }
    }
}

int main() {
    window = new sf::RenderWindow(sf::VideoMode(RES_X, RES_Y), "I love this Game");

    sf::Clock clock;
    float frameTime = 1/60.0f;
    float dTime = 0;

    initialiseGame();

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
            updateGame();
        }

        // Draw frame
        window->clear();
        drawGameFrame();
        window->display();
    }

    delete window;
    return 0;
}