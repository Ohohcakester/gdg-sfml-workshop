#ifndef CAMERA_H_
#define CAMERA_H_

extern void absToRel(float& x, float& y);
extern void relToAbs(float& x, float& y);


class Camera {
public:
    float x, y;

    Camera(int x, int y);
    void update();
};


#endif