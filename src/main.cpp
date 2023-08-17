
#include "ParticleLife.h"

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <iostream>
#include <cstdlib>
#include <regex>



int windowWidth;
int windowHeight;
int frameRate;

int count;
int size;

Camera2D camera;
ParticleLife particleLife;

Vector2 mouseWindowPosition;
Vector2 mouseWorldPosition;
float mouseWheelMovement;


void init();
void update();
void render();


int main()
{
    init();

    while (!WindowShouldClose())
        update(), render();

    CloseWindow();
    return EXIT_SUCCESS;
}

void update()
{
    // get mouse input
    mouseWindowPosition = GetMousePosition();
    mouseWorldPosition = GetScreenToWorld2D(mouseWindowPosition, camera);
    mouseWheelMovement = GetMouseWheelMove();

    // handle camera pan on right click + drag
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 scaledInvMouseDelta = Vector2Scale(GetMouseDelta(), -1.0f / camera.zoom);
        camera.target = Vector2Add(camera.target, scaledInvMouseDelta);
    }

    // handle camera zoom on scroll
    if (mouseWheelMovement != 0) {
        camera.target = GetScreenToWorld2D(camera.offset, camera);
        camera.zoom += (mouseWheelMovement * 2.0f);
        if (camera.zoom < 2.0f) camera.zoom = 2.0f;
    }

    // step over simulation
    particleLife.update();
}

void render()
{
    BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
            if (true)
                for (int i = 0; i <= size; i++)
                    DrawLine(2.0f * i, 0, 2.0f * i, 2.0f * size, DARKGRAY),
                    DrawLine(0, 2.0f * i, 2.0f * size, 2.0f * i, DARKGRAY);
            particleLife.draw();
        EndMode2D();
        DrawFPS(windowWidth - 80, 5);
    EndDrawing();
}

void init()
{
    // window
    windowWidth = 1600;
    windowHeight = 800;
    frameRate = 60;
    InitWindow(windowWidth, windowHeight, "Particle Life");
    SetTargetFPS(frameRate);

    // simulation
    count = 1024;
    size = 16;
    particleLife.init(count, size);

    // camera
    camera.offset = { windowWidth/2.0f, windowHeight/2.0f };
    camera.target = { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 20.0f;
}