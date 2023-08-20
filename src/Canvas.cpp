#include "Canvas.h"

#include "ParticleLife.h"

#include <raylib.h>
#include <rlgl.h>



Canvas::Canvas()
{

}

Canvas::~Canvas()
{
    UnloadTexture(particleTexture);
}

void Canvas::init(ParticleLife& particleLife)
{
    this->particleCount = particleLife.count;
    this->gridSize = particleLife.size;
    this->particleTypes = particleLife.types;
    this->particlePositions = particleLife.positions;
    this->particleVelocities = particleLife.newVelocities;
    this->particleColours = particleLife.colours;
    drawGrid = true;

    // circle texture setup
    Image temp = GenImageColor(64, 64, BLANK);
    ImageDrawCircle(&temp, 32, 32, 32, WHITE);
    particleTexture = LoadTextureFromImage(temp);
    UnloadImage(temp);
}

void Canvas::update(Camera2D& camera)
{
    // handle camera pan on right click + drag
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        camera.target = Vector2Add(camera.target, Vector2Scale(GetMouseDelta(), -1.0f / camera.zoom));

    // handle camera zoom on scroll
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        camera.target = GetScreenToWorld2D(camera.offset, camera);
        camera.zoom += (wheel * 2.0f);
        if (camera.zoom < 2.0f) camera.zoom = 2.0f;
    }
}

void Canvas::draw()
{
    if (drawGrid) {

        // prep opengl for dark grey drawing lines
        rlBegin(RL_LINES);
            rlColor4ub(80, 80, 80, 255);

            // draw grid
            for (int i = 0; i <= gridSize; i++)
                rlVertex2i(0, i*2), rlVertex2i(gridSize*2, i*2),    // horizontal line
                rlVertex2i(i*2, 0), rlVertex2i(i*2, gridSize*2);    // vertical line

    }

    // prep opengl for drawing particle texture to quads
    rlSetTexture(particleTexture.id);
    rlBegin(RL_QUADS);

        // for each particle
        for (int i = 0; i < particleCount; i++) {

            // cached variables
            float x = particlePositions[i].x;
            float y = particlePositions[i].y;
            Color colour = particleColours[particleTypes[i]];

            // set colour of particle and set normal towards viewer
            rlColor4ub(colour.r, colour.g, colour.b, 255);
            rlNormal3f(0.0f, 0.0f, 1.0f);

            // top left 
            rlTexCoord2f(0.0f, 0.0f);
            rlVertex2f(x-0.05f, y-0.05f);

            // bottom left
            rlTexCoord2f(0, 1.0f);
            rlVertex2f(x-0.05f, y+0.05f);

            // bottom right
            rlTexCoord2f(1.0f, 1.0f);
            rlVertex2f(x+0.05f, y+0.05f);

            // top right
            rlTexCoord2f(1.0f, 0.0f);
            rlVertex2f(x+0.05f, y-0.05f);

        }

    // reset current texture for opengl
    rlSetTexture(0);

    rlEnd();

}

void Canvas::randomisePositions()
{
    int scaledMax = 2*gridSize * 100;
    for (int i = 0; i < particleCount; i++)
        particlePositions[i] = { GetRandomValue(0, scaledMax)/100.0f,
                                 GetRandomValue(0, scaledMax)/100.0f };
}

void Canvas::randomiseVelocities()
{
    int scaledMax = 2*PI * 100;
    for (int i = 0; i < particleCount; i++) {
        float angle = GetRandomValue(0, scaledMax) / 100.f;
        particleVelocities[i] = { -cosf(angle), sinf(angle) };
    }
}

void Canvas::randomiseTypes()
{
    int dif = GetRandomValue(0, 2);
    for (int i = 0; i < particleCount; i++)
        particleTypes[i] = (i+dif) % 3;
}