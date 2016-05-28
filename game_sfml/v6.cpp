#include <SFML/Graphics.hpp>
#include <iostream>

/// Globals
sf::RenderWindow* window;
int RES_X = 800;
int RES_Y = 600;

bool key_left = false;
bool key_right = false;

int floorHeight = 500;

class Player;
class Bullet;
class Enemy;
Player* player;
std::vector<Bullet> bullets;
std::vector<Enemy> enemies;

int frame;

class Bullet {
public:
    float x, y;
    float vx, vy;
    int radius = 5;
    
    Bullet(int x, int y, float angle, float speed) {
        this->x = x;
        this->y = y;
        vx = speed*cos(angle);
        vy = speed*sin(angle);
    }

    void update() {
        x += vx;
        y += vy;
    }
    
    void draw() {
        sf::CircleShape shape;
        shape.setFillColor(sf::Color::Cyan);
        shape.setRadius(radius);
        shape.setOrigin(radius, radius);
        shape.setPosition(x, y);
        window->draw(shape);
    }
};

class Player {
public:
    int x, y;
    int width = 60;
    int height = 40;
    int speed = 5;
    float bulletSpeed = 25;
    
    Player(int x, int y): x(x), y(y) {}
    
    void update() {
        if (key_left) x -= speed;
        if (key_right) x += speed;
    }
    
    void draw() {
        sf::RectangleShape shape;
        shape.setFillColor(sf::Color::Blue);
        shape.setSize(sf::Vector2f(width, height));
        shape.setOrigin(width/2, height/2);
        shape.setPosition(x, y);
        window->draw(shape);
    }
    
    void shoot(int targetX, int targetY) {
        float angle = atan2(targetY - y, targetX - x);
        bullets.push_back(Bullet(x,y,angle,bulletSpeed));
    }
};


class Enemy {
public:
    float x, y, vx, vy, ax, ay;
    int radius = 35;
    
    Enemy(int x, int y, int vx, int vy) {
        this->x = x;
        this->y = y;
        this->vx = vx;
        this->vy = vy;
        this->ax = 0;
        this->ay = 0.07f;
    }
    
    void update() {
        vx += ax;
        vy += ay;
        x += vx;
        y += vy;
        
        if (vx < 0 && x < 0) {
            x = -x;
            vx = -vx;
        }
        if (vx > 0 && x > RES_X) {
            x = 2*RES_X - x;
            vx = -vx;
        }
        if (vy > 0 && y + radius > floorHeight) {
            y = 2*(floorHeight-radius) - y;
            vy = -vy;
        }
    }
    
    void draw() {
        sf::CircleShape shape;
        shape.setFillColor(sf::Color::Yellow);
        shape.setRadius(radius);
        shape.setOrigin(radius, radius);
        shape.setPosition(x, y);
        window->draw(shape);
    }
};


void drawFloor() {
    sf::RectangleShape shape;
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(0, floorHeight);
    shape.setSize(sf::Vector2f(RES_X, RES_Y-floorHeight));
    window->draw(shape);
}

void spawnEnemy() {
    int x = (rand()%2 == 0) ? -50 : RES_X + 50;
    int y = 100 + rand()%200;
    int vx = 1 + rand()%3;
    if (x > RES_X) vx = -vx;
    int vy = rand()%5 - 2;
    
    enemies.push_back(Enemy(x,y,vx,vy));
}

void initialiseGame() {
    if (player != nullptr) delete player;
    player = new Player(RES_X/2, floorHeight - 20);
    bullets.clear();
    enemies.clear();

    frame = 0;
}

void updateGame() {
    player->update();
    for (int i=0;i<bullets.size();++i) {
        bullets[i].update();
    }
    for (int i=0;i<enemies.size();++i) {
        enemies[i].update();
    }

    if (frame%80 == 0) {
        spawnEnemy();
    }
    frame++;
}

void drawGameFrame() {
    drawFloor();

    player->draw();
    for (int i=0;i<bullets.size();++i) {
        bullets[i].draw();
    }
    for (int i=0;i<enemies.size();++i) {
        enemies[i].draw();
    }
}

void keyDown(sf::Keyboard::Key keyCode) {
    switch(keyCode) {
        case sf::Keyboard::A: key_left = true; break;
        case sf::Keyboard::D: key_right = true; break;
    }
}

void keyUp(sf::Keyboard::Key keyCode) {
    switch(keyCode) {
        case sf::Keyboard::A: key_left = false; break;
        case sf::Keyboard::D: key_right = false; break;
    }
}

void mousePress(sf::Mouse::Button button, int x, int y) {
    switch(button) {
        case sf::Mouse::Left: {
            player->shoot(x,y);
            break;
        }
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
            mousePress(event.mouseButton.button, event.mouseButton.x, event.mouseButton.y);
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