#include "ParticleLife.h"

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <iostream>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"



int windowWidth;
int windowHeight;
int frameRate;

int count;
int size;
int bound;

Camera2D camera;
ParticleLife particleLife;


void init();
void update();
void render();



void init()
{
    // window
    windowWidth = 1600;
    windowHeight = 800;
    frameRate = 60;
    InitWindow(windowWidth, windowHeight, "Particle Life");
    SetTargetFPS(frameRate);

    // camera
    camera.offset = { windowWidth/2.0f, windowHeight/2.0f };
    camera.target = { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // simulation
    count = 1024;
    size = 16;
    bound = size/2;
    particleLife.init(count, size);
}

void update()
{
    // pan and zoom controls
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 scaledInvMouseDelta = Vector2Scale(GetMouseDelta(), -1.0f / camera.zoom);
        camera.target = Vector2Add(camera.target, scaledInvMouseDelta);
    }
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        camera.target = GetScreenToWorld2D(camera.offset, camera);
        camera.zoom += (wheel * 2.0f);
        if (camera.zoom < 2.0f)
            camera.zoom = 2.0f;
    }

    // step over simulation
    particleLife.update();
}

void render()
{
    BeginDrawing();
        ClearBackground(DARKGRAY);

        // grid and particles
        BeginMode2D(camera);
            for (int i = -bound; i <= bound; i++) {
                DrawLine(i*2.0f, -bound*2.0f, i*2.0f, bound*2.0f, GRAY);
                DrawLine(-bound*2.0f, i*2.0f, bound*2.0f, i*2.0f, GRAY);
            }
            particleLife.draw();
        EndMode2D();

        // user interface
        DrawFPS(0, 0);

    EndDrawing();
}



int main()
{
    init();

    while (!WindowShouldClose())
        update(), render();

    CloseWindow();
    return EXIT_SUCCESS;
}