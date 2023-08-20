#pragma once

#include "ParticleLife.h"

#include <raylib.h>
#include <rlgl.h>



class Canvas
{
 public:

    Canvas();
    ~Canvas();

    bool drawGrid;

    void init(ParticleLife& particleLife);
    void update(Camera2D& camera);
    void draw();

    void randomisePositions();
    void randomiseVelocities();
    void randomiseTypes();

 private:

    int particleCount;
    int gridSize;
    int* particleTypes;
    Vector2* particlePositions;
    Vector2* particleVelocities;
    Color* particleColours;
    Texture2D particleTexture;
    
};