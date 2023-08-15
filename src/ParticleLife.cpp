#include "ParticleLife.h"

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <iostream>



ParticleLife::ParticleLife()
{
    
}

ParticleLife::~ParticleLife()
{
    delete [] types;
    delete [] positions;
    delete [] oldVelocities;
    delete [] newVelocities;
}

void ParticleLife::init(int count, int size)
{
    // settings
    this->count = count;
    bounds = { (float)(size), (float)(size) };
    step = 0.005f;
    colours[0] = RED;   innerRadii[0] = 0.25f;  resistances[0] = 0.01f * innerRadii[0];
    colours[1] = GREEN; innerRadii[1] = 0.50f;  resistances[1] = 0.01f * innerRadii[1];
    colours[2] = BLUE;  innerRadii[2] = 0.75f;  resistances[2] = 0.01f * innerRadii[2];
    attractions[0][0] = -0.25f;  attractions[0][1] =  0.50f;  attractions[0][2] =  0.75f;
    attractions[1][0] =  0.75f;  attractions[1][1] = -0.25f;  attractions[1][2] =  0.50f;
    attractions[2][0] =  0.50f;  attractions[2][1] =  0.75f;  attractions[2][2] =  0.25f;
    
    // particle data
    types = new int[count];
    positions = new Vector2[count];
    oldVelocities = new Vector2[count];
    newVelocities = new Vector2[count];

    // randmouse type, position and velocity
    for (int i = 0; i < count; i++) {
        float angle = (float) (GetRandomValue(0, 360));
        types[i] = GetRandomValue(0, 2);
        positions[i].x = GetRandomValue(-bounds.x * 100, bounds.x * 100) / 100.0f;
        positions[i].y = GetRandomValue(-bounds.x * 100, bounds.x * 100) / 100.0f;
        oldVelocities[i].x = -cosf(angle);
        oldVelocities[i].y = sinf(angle);
        newVelocities[i] = oldVelocities[i];
    }
}

void ParticleLife::update()
{
    // for each particle
    for (int i = 0; i < count; i++) {
        int type = types[i];
        float innerRadius = innerRadii[type];
        float x = positions[i].x;
        float y = positions[i].y;

        // for each other particle
        for (int j = 0; j < count; j++) {
            if (i == j) continue;

            // get distance from other particle
            float xDist = positions[j].x - x;
            float yDist = positions[j].y - y;
            float distance = sqrtf(xDist*xDist + yDist*yDist);

            // if other particle within acting range
            if (distance <= 1.0f) {

                // get repulsions or attraction force from inner and outer radius cross over
                float reactionCoef = (distance <= innerRadius)
                    ? 1.0f - distance / innerRadius
                    : attractions[type][types[j]] * (distance - innerRadius);
                
                // apply normalised force new velocities
                float xForce = reactionCoef * xDist / distance;
                float yForce = reactionCoef * yDist / distance;
                newVelocities[i].x += xForce;
                newVelocities[i].y += yForce;
                newVelocities[j].x -= xForce;
                newVelocities[j].y -= yForce;
            }
        }
    }

    // for each particle
    for (int i = 0; i < count; i++) {

        // apply friction
        newVelocities[i].x *= 1.0f - resistances[types[i]];
        newVelocities[i].y *= 1.0f - resistances[types[i]];

        // apply movement
        positions[i].x += step * newVelocities[i].x;
        positions[i].y += step * newVelocities[i].y;

        // bounce if bounds reached
        if (abs(positions[i].x) > bounds.x) newVelocities[i].x *= -1.0f;
        if (abs(positions[i].y) > bounds.y) newVelocities[i].y *= -1.0f;
        
        // set velocities for next step
        oldVelocities[i].x = newVelocities[i].x;
        oldVelocities[i].y = newVelocities[i].y;
    }
}

void ParticleLife::draw()
{
    // for each particle
    float radius = 0.1f;
    for (int i = 0; i < count; i++) {
        int type = types[i];
        float x = positions[i].x;
        float y = positions[i].y;
        Color colour = colours[type];

        // draw fading circle size of particles influence
        rlBegin(RL_TRIANGLES);
            for (int j = 0; j < 360; j += 36) {
                rlColor4ub(colour.r, colour.g, colour.b, 255);
                rlVertex2f(x, y);
                rlColor4ub(colour.r, colour.g, colour.b, 0);
                rlVertex2f(x + sinf(DEG2RAD*j) * radius, y + cosf(DEG2RAD*j) * radius);
                rlColor4ub(colour.r, colour.g, colour.b, 0);
                rlVertex2f(x + sinf(DEG2RAD*(j+36)) * radius, y + cosf(DEG2RAD*(j+36)) * radius);
            }
        rlEnd();
    }
}
