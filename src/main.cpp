#include "ParticleLife.h"

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <iostream>
#include <cstdlib>
#include <regex>

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

Vector2 mouseScreenPosition;
Vector2 mouseWorldPosition;
float mouseWheelMovement;

bool* activeElementBoolPtr;
bool gridActive;
bool stepTextBoxActive;    char stepText[6];

Rectangle sidebarBounds;
Rectangle gridCheckBounds;

Rectangle stepLabelBounds, stepTextBoxBounds;


void init();
void update();
void render();

bool textIsFloat(char text[6]);
bool selectTextBox(Rectangle bounds, bool* isActivePtr);
bool submitTextBoxFloat(char text[6], float* valuePtr);
void ui();


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
    mouseScreenPosition = GetMousePosition();
    mouseWorldPosition = GetScreenToWorld2D(mouseScreenPosition, camera);
    mouseWheelMovement = GetMouseWheelMove();

    // step over simulation
    particleLife.update();
}

bool textIsFloat(char text[6])
{
    std::regex floatPattern("^[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?$");
    return std::regex_match(text, floatPattern);
}

bool selectTextBox(Rectangle bounds, bool* isActivePtr)
{
    bool activated = CheckCollisionPointRec(mouseScreenPosition, bounds);
    if (activated) {
        if (activeElementBoolPtr != nullptr)
            *activeElementBoolPtr = false;
        *isActivePtr = true;
        activeElementBoolPtr = isActivePtr;
    }
    return activated;
}

bool submitTextBoxFloat(char text[6], float* valuePtr)
{
    return true;
}

void ui()
{
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

    // sidebar option selection
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (selectTextBox(stepTextBoxBounds, &stepTextBoxActive));
        else if (activeElementBoolPtr != nullptr)
            *activeElementBoolPtr = false,
            activeElementBoolPtr = nullptr;
    }

    // handle textbox submission
    if ((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))) {
        if (activeElementBoolPtr != nullptr) {

        }
    }

    DrawFPS(windowWidth - 80, 5);

    DrawRectangleRec(sidebarBounds, LIGHTGRAY);

    GuiCheckBox(gridCheckBounds, "Draw Grid", &gridActive);


    GuiDrawText("Step", stepLabelBounds, TEXT_ALIGN_RIGHT, textIsFloat(stepText) ? DARKGRAY : RED);
    GuiTextBox(stepTextBoxBounds, stepText, 6, stepTextBoxActive);
}

void render()
{
    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
            if (gridActive)
                for (int i = -bound; i <= bound; i++)
                    DrawLine(2.0f*i, 2.0f*-bound, 2.0f*i, 2.0f*bound, DARKGRAY),
                    DrawLine(2.0f*-bound, 2.0f*i, 2.0f*bound, 2.0f*i, DARKGRAY);
            particleLife.draw();
        EndMode2D();

        ui();

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

    // ui active option
    activeElementBoolPtr = nullptr;
    gridActive = false;
    stepTextBoxActive = false;

    // ui bounds
    sidebarBounds = { 0, 0, 200, (float)(windowHeight) };
    gridCheckBounds = { 80, 80, 20, 20 };
    stepTextBoxBounds = { 80, 100, 100, 20 };
    stepLabelBounds = { 0, 100, 80, 20 };
}