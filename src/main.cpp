#include "ParticleLife.h"
#include "Canvas.h"
#include "Gui.h"

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <iostream>
#include <cstdlib>

#include "raygui.h"



int windowWidth;
int windowHeight;
int fpsLimit;

int count;
int size;

ParticleLife particleLife;
Camera2D camera;
Canvas canvas;
Gui gui;


void init();
void update();
void render();



int main()
{
    init();

    while (!WindowShouldClose())
        update(),
        render();

    CloseWindow();
    return EXIT_SUCCESS;
}

void update()
{
    canvas.update(camera);
    particleLife.update();
}

void render()
{
    BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
            canvas.draw();
        EndMode2D();
        gui.updateRender(particleLife, canvas);
        DrawFPS(windowWidth - 80, 5);
    EndDrawing();
}

void init()
{
    // window
    windowWidth = 1600;
    windowHeight = 800;
    fpsLimit = 0;
    InitWindow(windowWidth, windowHeight, "Particle Life");
    SetTargetFPS(fpsLimit);

    // simulation
    count = 1536;
    size = 12;
    particleLife.init(count, size);

    // camera
    camera.offset = { windowWidth/2.0f, windowHeight/2.0f };
    camera.target = { (float)(size), (float)(size) };
    camera.rotation = 0.0f;
    camera.zoom = 20.0f;

    // canvas
    canvas.init(particleLife);
    gui.init(particleLife, canvas);

}