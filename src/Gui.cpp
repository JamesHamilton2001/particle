#include "Gui.h"

#include <raylib.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

Gui::Gui()
{

}

Gui::~Gui()
{

}

void Gui::init(ParticleLife &particleLife, Canvas &canvas)
{
    // int windowWidth = GetScreenWidth();
    float windowHeight = GetScreenHeight();
    Rectangle boundsBuffer;

    // grid check box
    boundsBuffer = { 200, windowHeight - 250, 20, 20 };
    gridCheckBox.bounds     = boundsBuffer;
    gridCheckBox.label      = elementLabel(boundsBuffer, "DrawGrid");
    gridCheckBox.valuePtr   = &canvas.drawGrid;
}

void Gui::updateRender()
{
    GuiDrawText(gridCheckBox.label.text, gridCheckBox.label.bounds, TEXT_ALIGN_RIGHT, WHITE);
    GuiCheckBox(gridCheckBox.bounds, NULL, gridCheckBox.valuePtr);
}

Gui::Label Gui::elementLabel(Rectangle elementBounds, const char labelText[LABEL_BUF_LEN])
{
    Label label = { { elementBounds.x - 70, elementBounds.y, 60, 20 }, "" };
    std::strcpy(label.text, labelText);
    return label;
}