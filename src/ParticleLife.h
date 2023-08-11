#pragma once

#include <raylib.h>
#include <raymath.h>


class ParticleLife
{
 public:

    ParticleLife(int count, int size, int innerRange, int outerRange, float resistance);
    ~ParticleLife();

    void update();
    void draw();

 private:

    class Particle { public:
        Particle() {}
        int type; 
        Vector2 pos, vel;
    };

    int count;
    float innerRange;
    float outerRange;
    float diameter;
    float resistance;
    Particle* particles;

    Color colours[3] = { RED, GREEN, BLUE };
    Color rangeColours[3] = { RED, GREEN, BLUE };
    float attractions[3][3] = { 0.0f };

    Vector2 bounds;

};