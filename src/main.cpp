#include "ParticleLife.h"

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <iostream>



int frameRate = 60;
Vector2 screenSize = { 512, 512 };

Camera2D camera;
float zoomIncrement = 0.4f;

float radius = 2.0f;
float innerRange = 0.3f;
float outerRange = 1.0f;
float resistance = 0.15f;

int count = 512;
int size = 24;
int bound = size / 2;
float cellSize = outerRange * 2.0f;

ParticleLife particleLife(count, size, innerRange, outerRange, resistance);


int main()
{
    InitWindow(screenSize.x, screenSize.y, "Particle Life");
    SetTargetFPS(frameRate);

    camera.offset = { screenSize.x / 2.0f, screenSize.y / 2.0f };
    camera.target = { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose())
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 scaledInvMouseDelta = Vector2Scale(GetMouseDelta(), -1.0f / camera.zoom);
            camera.target = Vector2Add(camera.target, scaledInvMouseDelta);
        }
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            camera.target = GetScreenToWorld2D(camera.offset, camera);
            camera.zoom += (wheel * zoomIncrement);
            if (camera.zoom < zoomIncrement)
                camera.zoom = zoomIncrement;
        }

        particleLife.update(0.1f);

        BeginDrawing();
            ClearBackground(DARKGRAY);
            BeginMode2D(camera);
                for (int i = -bound; i <= bound; i++) {
                    DrawLine(i * cellSize, -bound * cellSize, i * cellSize, bound * cellSize, GRAY);
                    DrawLine(-bound * cellSize, i * cellSize, bound * cellSize, i * cellSize, GRAY);
                }
                particleLife.draw();
            EndMode2D();
            DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}