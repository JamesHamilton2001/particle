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
    delete [] velocities;
}

void ParticleLife::init(int count, int size)
{
    // simulation settings
    this->count = count;
    this->size = size;
    bounds = 2.0f * size;
    step = 0.00005f;

    // particle settings
    colours[0] = RED;
    colours[1] = GREEN;
    colours[2] = BLUE;
    innerRadii[0] = 0.25f;
    innerRadii[1] = 0.50f;
    innerRadii[2] = 0.75f;
    resistance = 0.001f;
    attractions[0][0] = -0.05f;  attractions[0][1] =  0.05f;  attractions[0][2] =  0.10f;
    attractions[1][0] =  0.10f;  attractions[1][1] = -0.05f;  attractions[1][2] =  0.05f;
    attractions[2][0] =  0.05f;  attractions[2][1] =  0.10f;  attractions[2][2] = -0.05f;

    // particle data
    types = new int[count];
    positions = new Vector2[count];
    velocities = new Vector2[count];

    // initial values
    for (int i = 0; i < count; i++) {
        types[i] = i%3;
        velocities[i] = { 0, 0 };
        positions[i] = { GetRandomValue(0, bounds*100) / 100.0f,
                         GetRandomValue(0, bounds*100) / 100.0f };
    }
}

void ParticleLife::update()
{
    calculateForces();
    applyForces();
}

void ParticleLife::calculateForces()
{
    // cache peak attraction points
    const float peaks[3] = {
        2.0f * (innerRadii[0] + (1.0f-innerRadii[0])/2.0f),
        2.0f * (innerRadii[1] + (1.0f-innerRadii[1])/2.0f),
        2.0f * (innerRadii[2] + (1.0f-innerRadii[2])/2.0f)
    };

    // for each particle
    for (int i = 0; i < count; i++) {

        // cache particle variables
        const int type = types[i];
        const float innerDiameter = 2.0f * innerRadii[type];
        const float xPos = positions[i].x;
        const float yPos = positions[i].y;
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

                // repulse if in inner radius
                float reactionCoef = (distance <= innerDiameter)
                    ? 1.0f - innerDiameter / distance
                    :  attractionArray[types[j]] * (distance-peaks[types[j]]);
                
                // apply normalised force to other particle
                float xForce = reactionCoef * xDist / distance;
                float yForce = reactionCoef * yDist / distance;
                xVelInc += xForce;
                yVelInc += yForce;
                velocities[j].x -= xForce;
                velocities[j].y -= yForce;
            }
        }

        // apply accumulative force to original particle
        velocities[i].x += xVelInc;
        velocities[i].y += yVelInc;
    }
}

void ParticleLife::applyForces()
{
    // cache inversed resistance
    const float invResistance = 1.0f - resistance;

    // for each particle
    for (int i = 0; i < count; i++) {

        // cache variables
        float xVelocity = velocities[i].x;
        float yVelocity = velocities[i].y;

        // apply friction
        xVelocity *= invResistance;
        yVelocity *= invResistance;

        // apply movement
        positions[i].x += step * xVelocity;
        positions[i].y += step * yVelocity;

        // bounce if bounds reached
        if (positions[i].x < 0 || positions[i].x > bounds)
            xVelocity *= -1.0f;
        if (positions[i].y < 0 || positions[i].y > bounds)
            yVelocity *= -1.0f;

        // apply calculated velocity
        velocities[i].x = xVelocity;
        velocities[i].y = yVelocity;
    }
}