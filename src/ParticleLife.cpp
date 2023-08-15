#include "ParticleLife.h"

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <iostream>



ParticleLife::ParticleLife(int count, int size, int innerRadius, int outerRadius, float resistance)
{
    this->count = count;
    this->innerRadius = innerRadius;
    this->outerRadius = outerRadius;
    this->resistance = resistance;
    this->bounds = { (float)(size*outerRadius), (float)(size*outerRadius) };
    
    types = new int[count];
    positions = new Vector2[count];
    oldVelocities = new Vector2[count];
    newVelocities = new Vector2[count];

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            attractions[i][j] = (float) (GetRandomValue(0, 100)) / 100.0f;

    for (int i = 0; i < count; i++) {
        float angle = (float) (GetRandomValue(0, 360));
        types[i] = GetRandomValue(0, 2);
        positions[i].x = GetRandomValue(-bounds.x * 100, bounds.x * 100) / 100.0f;
        positions[i].y = GetRandomValue(-bounds.x * 100, bounds.x * 100) / 100.0f;
        std::cout << positions[i].x << std::endl;
        std::cout << positions[i].y << std::endl << std::endl;
        oldVelocities[i].x = -cosf(angle);
        oldVelocities[i].y = sinf(angle);
        newVelocities[i] = oldVelocities[i];
    }
}

ParticleLife::~ParticleLife()
{
    delete [] types;
    delete [] positions;
    delete [] oldVelocities;
    delete [] newVelocities;
}

void ParticleLife::update(float step)
{
    // for each particle
    for (int i = 0; i < count; i++) {

        // for each other particle
        for (int j = 0; j < count; j++) {
            if (i == j) continue;

            // get distance from other particle
            float xDist = positions[j].x - positions[i].x;
            float yDist = positions[j].y - positions[i].y;
            float distance = sqrtf(xDist*xDist + yDist*yDist);

            // if other particle within acting range
            if (distance <= outerRadius) {

                // get repulsions or attraction force from inner and outer radius cross over
                float reactionCoef = (distance <= innerRadius)
                    ? 1.0f - distance / innerRadius
                    : attractions[types[i]][types[j]] * (distance - innerRadius) / outerRadius;
                
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
        newVelocities[i].x *= 1.0f - resistance;
        newVelocities[i].y *= 1.0f - resistance;

        // // bounce if bounds reached
        if (abs(positions[i].x) > bounds.x) {
            newVelocities[i].x *= -1.0f;
            positions[i].x = (positions[i].x > 0) ? bounds.x : -bounds.x;
        } if (abs(positions[i].y) > bounds.y) {
            newVelocities[i].y *= -1.0f;
            positions[i].y = (positions[i].y > 0) ? bounds.y : -bounds.y;
        }

        // increment position by new velocity vector
        positions[i].x += newVelocities[i].x;
        positions[i].y += newVelocities[i].y;
        
        // set velocities for next step
        oldVelocities[i].x = newVelocities[i].x;
        oldVelocities[i].y = newVelocities[i].y;
    }
}

void ParticleLife::draw()
{
    for (int i = 0; i < count; i++) {
        Color colour = colours[types[i]];
        float x = positions[i].x;
        float y = positions[i].y;

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
