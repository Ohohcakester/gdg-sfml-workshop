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
class Camera;
class Sfx;
Player* player;
Camera* camera;
std::vector<Bullet> bullets;
std::vector<Enemy> enemies;
std::vector<Sfx*> sfxs;
void absToRel(float& x, float& y);
void relToAbs(float& x, float& y);

int frame;

class Sfx {
public:
    virtual ~Sfx(){}

    bool isActive = true;
    int animframe;
    float x, y, vx, vy, ax, ay;

    Sfx(float x, float y, float vx=0, float vy=0, float ax=0, float ay=0) {
        this->x = x;
        this->y = y;
        this->vx = vx;
        this->vy = vy;
        this->ax = ax;
        this->ay = ay;
        this->animframe = 0;
    }

    void sfxUpdate() {
        if (!isActive) return;

        vx += ax;
        vy += ay;
        x += vx;
        y += vy;
        animframe++;
    }

    virtual void update() = 0;
    virtual void draw() = 0;
};

class BulletPop : public Sfx {
public:
    int duration = 20;

    BulletPop(float x, float y): Sfx(x,y) {}

    void update() {
        if (!isActive) return;

        sfxUpdate();
        if (animframe >= duration) isActive = false;
    }

    void draw() {
        if (!isActive) return;
        float radius = 1 + 60*animframe/duration;

        sf::CircleShape shape;
        shape.setFillColor(sf::Color::Transparent);
        shape.setRadius(radius);
        shape.setOutlineThickness(3);
        shape.setOutlineColor(sf::Color::Cyan);
        shape.setOrigin(radius, radius);
        float drawX = x, drawY = y;
        absToRel(drawX, drawY);
        shape.setPosition(drawX, drawY);
        window->draw(shape);
    }
};

class Bullet {
public:
    bool isActive = true;
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
        if (!isActive) return;

        x += vx;
        y += vy;
    }
    
    void draw() {
        if (!isActive) return;

        sf::CircleShape shape;
        shape.setFillColor(sf::Color::Cyan);
        shape.setRadius(radius);
        shape.setOrigin(radius, radius);
        float drawX = x, drawY = y;
        absToRel(drawX, drawY);
        shape.setPosition(drawX, drawY);
        window->draw(shape);
    }
};

class Player {
public:
    bool isActive = true;
    int x, y;
    int width = 60;
    int height = 40;
    int speed = 5;
    int hitboxRadius = 10;
    float bulletSpeed = 25;
    
    Player(int x, int y): x(x), y(y) {}
    
    void update() {
        if (!isActive) return;

        if (key_left) x -= speed;
        if (key_right) x += speed;
    }
    
    void draw() {
        if (!isActive) return;

        sf::RectangleShape shape;
        shape.setFillColor(sf::Color::Blue);
        shape.setSize(sf::Vector2f(width, height));
        shape.setOrigin(width/2, height/2);
        float drawX = x, drawY = y;
        absToRel(drawX, drawY);
        shape.setPosition(drawX, drawY);
        window->draw(shape);
    }
    
    void shoot(int targetX, int targetY) {
        if (!isActive) return;

        float angle = atan2(targetY - y, targetX - x);
        bullets.push_back(Bullet(x,y,angle,bulletSpeed));
    }
};


class Enemy {
public:
    bool isActive = true;
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
        if (!isActive) return;

        vx += ax;
        vy += ay;
        x += vx;
        y += vy;
        
        if (vy > 0 && y + radius > floorHeight) {
            y = 2*(floorHeight-radius) - y;
            vy = -vy;
        }

        checkCollisions();
    }
    
    void draw() {
        if (!isActive) return;

        sf::CircleShape shape;
        shape.setFillColor(sf::Color::Yellow);
        shape.setRadius(radius);
        shape.setOrigin(radius, radius);
        float drawX = x, drawY = y;
        absToRel(drawX, drawY);
        shape.setPosition(drawX, drawY);
        window->draw(shape);
    }

    bool isCollidingWith(Bullet& bullet) {
        if (!bullet.isActive) return false;
    
        float dx = x - bullet.x;
        float dy = y - bullet.y;
        float r = radius + bullet.radius;
        return dx*dx + dy*dy <= r*r;
    }
    
    bool isCollidingWith(Player* player) {
        if (!player->isActive) return false;
    
        float dx = x - player->x;
        float dy = y - player->y;
        float r = radius + player->hitboxRadius;
        return dx*dx + dy*dy <= r*r;
    
    }
    
    void checkCollisions() {
        for (int i=0;i<bullets.size();++i) {
            if (isCollidingWith(bullets[i])) {
                sfxs.push_back(new BulletPop(bullets[i].x, bullets[i].y));

                bullets[i].isActive = false;
                this->isActive = false;
            }
        }
        if (isCollidingWith(player)) {
            player->isActive = false;
        }
    }
};

class Camera {
public:
    float x, y;

    Camera(int x, int y) {
        this->x = x;
        this->y = y;
    }

    void update() {
        float p = 0.1f;

        x += p * (player->x - x);
        y += p * (player->y - y);
    }
};

void absToRel(float& x, float& y) {
    x -= camera->x - RES_X/2;
    y -= camera->y - floorHeight;
}

void relToAbs(float& x, float& y) {
    x += camera->x - RES_X/2;
    y += camera->y - floorHeight;
}

void drawFloor() {
    sf::RectangleShape shape;
    shape.setFillColor(sf::Color::Green);
    float drawX = 0, drawY = floorHeight;
    absToRel(drawX, drawY);
    shape.setPosition(0, drawY);
    shape.setSize(sf::Vector2f(RES_X, RES_Y-floorHeight));
    window->draw(shape);
}

void spawnEnemy() {
    int x = player->x + RES_X + 50;
    int y = 100 + rand()%200;
    int vx = - 1 - rand()%3;
    int vy = rand()%5 - 2;
    
    enemies.push_back(Enemy(x,y,vx,vy));
}

void initialiseGame() {
    if (player != nullptr) delete player;
    if (camera != nullptr) delete camera;
    player = new Player(RES_X/2, floorHeight - 20);
    camera = new Camera(player->x, player->y);
    bullets.clear();
    enemies.clear();

    for (int i=0;i<sfxs.size();++i) {
        delete sfxs[i];
    }
    sfxs.clear();

    frame = 0;
}

void updateGame() {
    player->update();
    camera->update();
    for (int i=0;i<bullets.size();++i) {
        bullets[i].update();
    }
    for (int i=0;i<enemies.size();++i) {
        enemies[i].update();
    }
    for (int i=0;i<sfxs.size();++i) {
        sfxs[i]->update();
    }

    if (frame%40 == 0) {
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
    for (int i=0;i<sfxs.size();++i) {
        sfxs[i]->draw();
    }
}

void keyDown(sf::Keyboard::Key keyCode) {
    switch(keyCode) {
        case sf::Keyboard::A: key_left = true; break;
        case sf::Keyboard::D: key_right = true; break;
        case sf::Keyboard::R: {
            initialiseGame();
            break;
        }
    }
}

void keyUp(sf::Keyboard::Key keyCode) {
    switch(keyCode) {
        case sf::Keyboard::A: key_left = false; break;
        case sf::Keyboard::D: key_right = false; break;
    }
}

void mousePress(sf::Mouse::Button button, int x, int y) {
    float absX = x, absY = y;
    relToAbs(absX, absY);

    switch(button) {
        case sf::Mouse::Left: {
            player->shoot(absX, absY);
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