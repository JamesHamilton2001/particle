#pragma once

#include "ParticleLife.h"

#include <raylib.h>
#include <rlgl.h>



class Canvas
{
 public:

    Canvas();
    ~Canvas();

    void init(ParticleLife& particleLife);
    void update(Camera2D& camera);
    void draw();

 private:

    int particleCount;
    int gridSize;
    int* particleTypes;
    Vector2* particlePositions;
    Color* particleColours;
    Texture2D particleTexture;

    bool drawGrid;
    
};