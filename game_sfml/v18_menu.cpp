#include <SFML/Graphics.hpp>
#include <iostream>

/// Globals
sf::RenderWindow* window;
int RES_X = 800;
int RES_Y = 600;

bool key_left = false;
bool key_right = false;

int floorHeight = 500;

bool inGame;


// Menu Variables
enum MenuScreen {
    menuscreen_main,
    menuscreen_options
};

enum MenuMainOption {
    menumain_startgame,
    menumain_options
};

int currentScreen = menuscreen_main;
int menuMainOption = menumain_startgame;
int difficultyLevel = 1;


// Game Variables
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
int enemySpawnInterval;

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

class ExplodeParticle : public Sfx {
public:
    int duration = 40;

    ExplodeParticle(float x, float y) : Sfx(x,y) {
        vx = (rand()%51 - 25)/5.0f;
        vy = (rand()%51 - 25)/5.0f;
        ay = 0.07f;
        duration = 30 + rand()%20;
    }

    void update() {
        if (!isActive) return;

        sfxUpdate();
        if (animframe >= duration) isActive = false;
    }

    void draw() {
        if (!isActive) return;

        int radius = 5;
        sf::CircleShape shape;
        shape.setFillColor(sf::Color::Yellow);
        shape.setRadius(radius);
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
                for (int j=0;j<10;++j) sfxs.push_back(new ExplodeParticle(x,y));

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
    inGame = true;

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
    
    switch(difficultyLevel) {
        case 1: enemySpawnInterval = 80; break;
        case 2: enemySpawnInterval = 60; break;
        case 3: enemySpawnInterval = 50; break;
        case 4: enemySpawnInterval = 40; break;
        case 5: enemySpawnInterval = 25; break;
        case 6: enemySpawnInterval = 10; break;
    }
}

void initialiseMenu() {
    inGame = false;
    currentScreen = menuscreen_main;
}

void updateGame() {
    player->update();
    camera->update();

    int numBullets = 0;
    for (int i=0;i<bullets.size();++i) {
        bullets[i].update();
        if (bullets[i].isActive) numBullets++;
    }
    if (numBullets * 2 < bullets.size()) {
        // Cleanup array.
        std::vector<Bullet> newBullets;
        newBullets.reserve(numBullets);
        for (int i=0;i<bullets.size();++i) {
            if (bullets[i].isActive) {
                newBullets.push_back(bullets[i]);
            }
        }
        bullets.swap(newBullets);
    }

    int numEnemies = 0;
    for (int i=0;i<enemies.size();++i) {
        enemies[i].update();
        if (enemies[i].isActive) numEnemies++;
    }
    if (numEnemies * 2 < enemies.size()) {
        // Cleanup array.
        std::vector<Enemy> newEnemies;
        newEnemies.reserve(numEnemies);
        for (int i=0;i<enemies.size();++i) {
            if (enemies[i].isActive) {
                newEnemies.push_back(enemies[i]);
            }
        }
        enemies.swap(newEnemies);
    }

    int numSfxs = 0;
    for (int i=0;i<sfxs.size();++i) {
        sfxs[i]->update();
        if (sfxs[i]->isActive) numSfxs++;
    }
    if (numSfxs * 2 < sfxs.size()) {
        // Cleanup array.
        std::vector<Sfx*> newSfxs;
        newSfxs.reserve(numSfxs);
        for (int i=0;i<sfxs.size();++i) {
            if (sfxs[i]->isActive) {
                newSfxs.push_back(sfxs[i]);
            } else {
                delete sfxs[i];
            }
        }
        sfxs.swap(newSfxs);
    }

    if (frame%enemySpawnInterval == 0) {
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

void updateMenu() {

}

void drawRectangle(int x1, int y1, int x2, int y2, sf::Color color) {
    sf::RectangleShape shape;
    shape.setFillColor(color);
    shape.setPosition(x1,y1);
    shape.setSize(sf::Vector2f(x2-x1, y2-y1));
    window->draw(shape);
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
        
        if (menuMainOption == i) {
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
        
        if (difficultyLevel == i+1) {
            drawRectangle(x-highlightHalfSize, y-highlightHalfSize, x+highlightHalfSize, y+highlightHalfSize, sf::Color::Yellow);
        }
        
        drawRectangle(x-buttonHalfSize, y-buttonHalfSize, x+buttonHalfSize, y+buttonHalfSize, sf::Color::Blue);
    }
}

void drawMenuFrame() {
    window->clear(sf::Color(64,64,127));

    switch (currentScreen) {
        case menuscreen_main: drawMenuMain(); break;
        case menuscreen_options: drawMenuOptions(); break;
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


void keypressMenuMain(sf::Keyboard::Key keyCode) {
    switch (keyCode) {
        case sf::Keyboard::Space: {
            switch(menuMainOption) {
            case menumain_startgame:
                initialiseGame();
                break;
            case menumain_options:
                currentScreen = menuscreen_options;
                break;
            }
            break;
        }
        case sf::Keyboard::W: {
            --menuMainOption;
            if (menuMainOption < 0) menuMainOption = 1;
            break;
        }
        case sf::Keyboard::S: {
            ++menuMainOption;
            if (menuMainOption > 1) menuMainOption = 0;
            break;
        }
    }
}

void keypressMenuOptions(sf::Keyboard::Key keyCode) {
    switch (keyCode) {
        case sf::Keyboard::A: {
            --difficultyLevel;
            if (difficultyLevel < 1) difficultyLevel = 6;
            break;
        }
        case sf::Keyboard::D: {
            ++difficultyLevel;
            if (difficultyLevel > 6) difficultyLevel = 1;
            break;
        }
        case sf::Keyboard::Space:
        case sf::Keyboard::Escape: {
            currentScreen = menuscreen_main;
            break;
        }
    }
}

void keyPressMenu(sf::Keyboard::Key keyCode) {
    switch (currentScreen) {
        case menuscreen_main: keypressMenuMain(keyCode); break;
        case menuscreen_options: keypressMenuOptions(keyCode); break;
    }
}    

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

void mousePressGame(sf::Mouse::Button button, int x, int y) {
    float absX = x, absY = y;
    relToAbs(absX, absY);

    switch(button) {
        case sf::Mouse::Left: {
            player->shoot(absX, absY);
            break;
        }
    }
}

void mousePressMenu(sf::Mouse::Button button, int x, int y) {
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