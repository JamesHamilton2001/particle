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
    // delete [] types;
    // delete [] positions;
    // delete [] velocities;

    // for (int i = 0; i < size; i++) {
    //     delete [] gridCounts[i];
    //     delete [] gridIndexes[i];
    // }
    // delete [] gridCounts;
    // delete [] gridIndexes;
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

    // 1st dimension setup
    gridCounts = new int*[size];
    gridIndexes = new std::vector<int>*[size];
    neighborhoods = new std::vector<int>***[size];

    // 2nd dimension setup
    for (int i = 0; i < size; i++) {
        gridCounts[i] = new int[size];
        gridIndexes[i] = new std::vector<int>[size];
        neighborhoods[i] = new std::vector<int>**[size];

        // 3rd dimension setup and defaults for neighborhoods
        for (int j = 0; j < size; j++) {
            neighborhoods[i][j] = new std::vector<int>*[8];
            neighborhoods[i][j][0] = &gridIndexes[(i+size-1)%size][(j+size-1)%size];    // top left
            neighborhoods[i][j][1] = &gridIndexes[i][(j+size-1)%size];                  // top middle
            neighborhoods[i][j][2] = &gridIndexes[(i+size+1)%size][(j+size-1)%size];    // top right
            neighborhoods[i][j][3] = &gridIndexes[i][(j+size-1)%size];                  // middle left
            neighborhoods[i][j][4] = &gridIndexes[i][(j+size+1)%size];                  // middle left
            neighborhoods[i][j][5] = &gridIndexes[(i+size-1)%size][(j+size+1)%size];    // bottom left
            neighborhoods[i][j][6] = &gridIndexes[i][(j+size+1)%size];                  // bottom middle
            neighborhoods[i][j][7] = &gridIndexes[(i+size+1)%size][(j+size+1)%size];    // bottom right
        }

        // count and index default values
        for (int j = 0; j < size; j++) {
            gridCounts[i][j] = 0;
            gridIndexes[i][j].resize(16);
            for (int k = 0; k < 16; k++)
                gridIndexes[i][j][k] = -1;
        }
    }

    // initial values
    for (int i = 0; i < count; i++) {
        types[i] = i % 3;
        velocities[i] = { 0, 0 };
        positions[i] = { GetRandomValue(0, bounds*100) / 100.0f,
                         GetRandomValue(0, bounds*100) / 100.0f };
    }
}

void ParticleLife::update()
{
    mapNeighborhoods();
    calculateForces();
    applyForces();
}

int ParticleLife::rowColHash(float coord)
{
    return (int)(coord/2.0f + size) % size;
}

void ParticleLife::mapNeighborhoods()
{
    // reset grid values
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            for (int i = 0; i < gridCounts[r][c]; i++)
                gridIndexes[r][c][i] = -1;
            gridCounts[r][c] = 0;
        }
    }

    // recalculate grid values
    for (int i = 0; i < count; i++) {
        int r = rowColHash(positions[i].y);
        int c = rowColHash(positions[i].x);
        gridIndexes[r][c][gridCounts[r][c]++] = i;
    }
}

void ParticleLife::calculateForces()
{
    // cache peak attraction points
    const float peaks[3] = {
        2.0f * (innerRadii[0] + (1.0f-innerRadii[0]) / 2.0f),
        2.0f * (innerRadii[1] + (1.0f-innerRadii[1]) / 2.0f),
        2.0f * (innerRadii[2] + (1.0f-innerRadii[2]) / 2.0f)
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

                // repulse if in inner radius, otherwise attract
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