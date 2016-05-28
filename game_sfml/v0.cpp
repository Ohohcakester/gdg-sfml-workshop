#include <SFML/Graphics.hpp>

/// Globals
sf::RenderWindow* window;
int RES_X = 800;
int RES_Y = 600;

void processEvent(sf::Event event) {
    switch(event.type) {
        case sf::Event::Closed: {
            window->close();
            break;
        }
    }
}

int main() {
    window = new sf::RenderWindow(sf::VideoMode(RES_X, RES_Y), "I love this Game");
    while(window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            processEvent(event);
        }
    }

    delete window;
    return 0;
}