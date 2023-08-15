#pragma once

#include <raylib.h>
#include <raymath.h>


class ParticleLife
{
 public:

    ParticleLife(int count, int size, int innerRange, int outerRange, float resistance);
    ~ParticleLife();

    void update(float step);
    void draw();

 private:

    int count;
    float innerRadius;
    float outerRadius;
    float resistance;
    Vector2 bounds;

    Color colours[3] = { RED, GREEN, BLUE };
    float attractions[3][3] = { 0.0f };

    int* type;
    Vector2* position;
    Vector2* oldVelocity;
    Vector2* newVelocity;

};