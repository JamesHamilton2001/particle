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
    boundsBuffer = { 100, windowHeight - 250, 20, 20 };
    gridCheckBox.bounds = boundsBuffer;
    gridCheckBox.valuePtr = &canvas.drawGrid;
    gridLabel = elementLabel(boundsBuffer, "DrawGrid", TEXT_ALIGN_RIGHT);
}

void Gui::updateRender()
{
    handleLable(gridLabel);
    handleCheckBox(gridCheckBox);
}

Gui::Label Gui::elementLabel(Rectangle elementBounds, const char labelText[LABEL_BUF_LEN], GuiTextAlignment alignment)
{
    Label label = { { elementBounds.x - 70, elementBounds.y, 60, 20 }, "", alignment };
    std::strcpy(label.text, labelText);
    return label;
}

void Gui::handleLable(Label& label)
{
    GuiDrawText(label.text, label.bounds, label.alignment, WHITE);
}

void Gui::handleCheckBox(CheckBox& checkBox)
{
    GuiCheckBox(checkBox.bounds, NULL, checkBox.valuePtr);
}

