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
    delete [] newVelocities;
}

void ParticleLife::init(int count, int size)
{
    // settings
    this->count = count;
    this->size = size;
    bounds = 2.0f * size;
    step = 0.00005f;
    colours[0] = RED;
    colours[1] = GREEN;
    colours[2] = BLUE;
    innerRadii[0] = 0.25f;
    innerRadii[1] = 0.50f;
    innerRadii[2] = 0.75f;
    resistance = 0.0022f;
    attractions[0][0] = -0.05f;  attractions[0][1] =  0.05f;  attractions[0][2] =  0.10f;
    attractions[1][0] =  0.10f;  attractions[1][1] = -0.05f;  attractions[1][2] =  0.05f;
    attractions[2][0] =  0.05f;  attractions[2][1] =  0.10f;  attractions[2][2] = -0.05f;

    // particle data
    types = new int[count];
    positions = new Vector2[count];
    newVelocities = new Vector2[count];

    randomise();
}

void ParticleLife::update()
{
    // cache resistance values
    float resistances[3] = {
        resistance*innerRadii[0],
        resistance*innerRadii[1],
        resistance*innerRadii[2]
    };

    // for each particle
    for (int i = 0; i < count; i++) {

        // cached variables
        int type = types[i];
        float innerRadius = innerRadii[type];
        float xPos = positions[i].x;
        float yPos = positions[i].y;
        float xVelInc = 0.0f;
        float yVelInc = 0.0f;

        // for each other particle
        for (int j = 0; j < count; j++) {
            if (i == j) continue;

            // get distance from other particle
            float xDist = positions[j].x - xPos;
            float yDist = positions[j].y - yPos;
            float sqDist = xDist*xDist + yDist*yDist;
            float* attractionArray = attractions[type];

            // if other particle within acting range
            if (sqDist <= 4.0f) {
                float distance = sqrtf(sqDist);

                // get repulsions or attraction force from inner and outer radius cross over
                float reactionCoef = (distance <= innerRadius)
                    ? 1.0f - innerRadius / distance
                    : attractionArray[types[j]] * (2.0f - distance);
                
                // apply normalised force to other particle
                float xForce = reactionCoef * xDist / distance;
                float yForce = reactionCoef * yDist / distance;
                xVelInc += xForce;
                yVelInc += yForce;
                newVelocities[j].x -= xForce;
                newVelocities[j].y -= yForce;
            }
        }
        newVelocities[i].x += xVelInc;
        newVelocities[i].y += yVelInc;
    }

    // for each particle
    for (int i = 0; i < count; i++) {

        // cache variables
        float resistance = 1.0f - resistances[types[i]];
        float xVelocity = newVelocities[i].x;
        float yVelocity = newVelocities[i].y;

        // apply friction
        xVelocity *= resistance;
        yVelocity *= resistance;

        // apply movement
        positions[i].x += step * xVelocity;
        positions[i].y += step * yVelocity;

        // bounce if bounds reached
        if (positions[i].x < 0 || positions[i].x > bounds)
            xVelocity *= -1.0f;
        if (positions[i].y < 0 || positions[i].y > bounds)
            yVelocity *= -1.0f;

        // apply calculated velocity
        newVelocities[i].x = xVelocity;
        newVelocities[i].y = yVelocity;
    }
}

void ParticleLife::randomise()
{
    for (int i = 0; i < count; i++) {
        float angle = (float) (GetRandomValue(0, 360));
        types[i] = GetRandomValue(0, 2);
        positions[i].x = GetRandomValue(0, bounds * 100) / 100.0f;
        positions[i].y = GetRandomValue(0, bounds * 100) / 100.0f;
        newVelocities[i].x = -cosf(angle);
        newVelocities[i].y = sinf(angle);
    }
}