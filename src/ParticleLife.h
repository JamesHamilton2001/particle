#pragma once

#include <raylib.h>
#include <raymath.h>


class ParticleLife
{
public:

    ParticleLife();
    ~ParticleLife();

    void init(int count, int size);
    void update();
    void draw();

    float step;
    float innerRadii[3];
    float resistances[3];
    float attractions[3][3];
    Color colours[3];

private:

    int count;
    Vector2 bounds;

    int* types;
    Vector2* positions;
    Vector2* oldVelocities;
    Vector2* newVelocities;

};