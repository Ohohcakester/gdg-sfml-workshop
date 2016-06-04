#ifndef SFX_H_
#define SFX_H_


class Sfx {
public:
    virtual ~Sfx(){}

    bool isActive = true;
    int animframe;
    float x, y, vx, vy, ax, ay;

    Sfx(float x, float y, float vx=0, float vy=0, float ax=0, float ay=0);
    void sfxUpdate();

    virtual void update() = 0;
    virtual void draw() = 0;
};

class BulletPop : public Sfx {
public:
    int duration = 20;

    BulletPop(float x, float y): Sfx(x,y) {}

    void update();
    void draw();
};

class ExplodeParticle : public Sfx {
public:
    int duration = 40;

    ExplodeParticle(float x, float y);

    void update();
    void draw();
};


#endif