#pragma once

#include <raylib.h>
#include <raymath.h>

/* NOTES
    - the infuential radius of a particle (outer radius) is 1.0f
    - resistance of a particle is scaled by the inner radius
    - a negative attraction value repulses
    - the simulations grid is the +/- of the bounds, particles will bounce of the edges
    - certain attributes are public to allow them to be manipulated using the ui
*/


class ParticleLife
{
public:

    ParticleLife();
    ~ParticleLife();

    void init(int count, int size);
    void update();

    int count;
    int size;
    float bounds;

    int* types;
    Vector2* positions;
    Vector2* newVelocities;

    float step;
    float innerRadii[3];
    float resistances[3];
    float attractions[3][3];
    Color colours[3];

};


