#include "ParticleLife.h"

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <iostream>



ParticleLife::ParticleLife(int count, int size, int innerRadius, int outerRadius, float resistance)
{
    // settings
    this->count = count;
    this->innerRadius = innerRadius;
    this->outerRadius = outerRadius;
    this->resistance = resistance;
    this->bounds = { (float)(size*outerRadius), (float)(size*outerRadius) };
    
    // particle data
    type = new int[count];
    position = new Vector2[count];
    oldVelocity = new Vector2[count];
    newVelocity = new Vector2[count];

    // randomise attraction values
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            attractions[i][j] = (float) (GetRandomValue(0, 100)) / 100.0f;

    // randmouse type, position and velocity
    for (int i = 0; i < count; i++) {
        float angle = (float) (GetRandomValue(0, 360));
        type[i] = GetRandomValue(0, 2);
        position[i].x = GetRandomValue(-bounds.x * 100, bounds.x * 100) / 100.0f;
        position[i].y = GetRandomValue(-bounds.x * 100, bounds.x * 100) / 100.0f;
        oldVelocity[i].x = -cosf(angle);
        oldVelocity[i].y = sinf(angle);
        newVelocity[i] = oldVelocity[i];
    }
}

ParticleLife::~ParticleLife()
{
    delete [] type;
    delete [] position;
    delete [] oldVelocity;
    delete [] newVelocity;
}

void ParticleLife::update(float step)
{
    // for each particle
    for (int i = 0; i < count; i++) {

        // for each other particle
        for (int j = 0; j < count; j++) {
            if (i == j) continue;

            // get distance from other particle
            float xDist = position[j].x - position[i].x;
            float yDist = position[j].y - position[i].y;
            float distance = sqrtf(xDist*xDist + yDist*yDist);

            // if other particle within acting range
            if (distance <= outerRadius) {

                // get repulsions or attraction force from inner and outer radius cross over
                float reactionCoef = (distance <= innerRadius)
                    ? 1.0f - distance / innerRadius
                    : attractions[type[i]][type[j]] * (distance - innerRadius) / outerRadius;
                
                // apply normalised force new velocities
                float xForce = reactionCoef * xDist / distance;
                float yForce = reactionCoef * yDist / distance;
                newVelocity[i].x += xForce;
                newVelocity[i].y += yForce;
                newVelocity[j].x -= xForce;
                newVelocity[j].y -= yForce;
            }
        }
    }

    // for each particle
    for (int i = 0; i < count; i++) {

        // bounce if bounds reached
        if (abs(position[i].x) > bounds.x) {
            newVelocity[i].x *= -1.0f;
            position[i].x = (position[i].x > 0) ? bounds.x : -bounds.x;
        } if (abs(position[i].y) > bounds.y) {
            newVelocity[i].y *= -1.0f;
            position[i].y = (position[i].y > 0) ? bounds.y : -bounds.y;
        }

        // apply friction
        newVelocity[i].x *= 1.0f - resistance;
        newVelocity[i].y *= 1.0f - resistance;

        // apply movement
        position[i].x += newVelocity[i].x;
        position[i].y += newVelocity[i].y;
        
        // set velocities for next step
        oldVelocity[i].x = newVelocity[i].x;
        oldVelocity[i].y = newVelocity[i].y;
    }
}

void ParticleLife::draw()
{
    // for each particle
    for (int i = 0; i < count; i++) {
        Color colour = colours[type[i]];
        float x = position[i].x;
        float y = position[i].y;

        // draw fading circle size of particles influence
        rlBegin(RL_TRIANGLES);
            for (int j = 0; j < 360; j += 36) {
                rlColor4ub(colour.r, colour.g, colour.b, 255);
                rlVertex2f(x, y);
                rlColor4ub(colour.r, colour.g, colour.b, 0);
                rlVertex2f(x + sinf(DEG2RAD*j) * outerRadius, y + cosf(DEG2RAD*j) * outerRadius);
                rlColor4ub(colour.r, colour.g, colour.b, 0);
                rlVertex2f(x + sinf(DEG2RAD*(j+36)) * outerRadius, y + cosf(DEG2RAD*(j+36)) * outerRadius);
            }
        rlEnd();
    }
}
