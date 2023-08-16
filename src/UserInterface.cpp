#include "UserInterface.h"

#include <raylib.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <regex>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"




UserInterface::UserInterface()
{

}

void UserInterface::init(
    int windowWidth,
    int windowHeight,
    float* stepPtr,
    float innerRadii[3],
    float resistances[3],
    float attractions[3][3],
    Color colours[3]
) {
    sideBarBounds = { 0, 0, 200, (float)(windowHeight) };

    float x, y, w, h;
    
    x = 80, y = 80, w = 20, h = 20;
    gridBoolCheckBox.init("Grid", { x, y, w, h });

    y += 30, w = 100;
    stepTextBox.init("Step", { x, y, w, h }, stepPtr, 0.0, 10000.0f);

    char rgb[3][1+1] = { "R", "G", "B" };
    w = 40;
    for (int i = 0; i < 3; i++) {
        innerRadiusTextBoxes[i].init(rgb[i], { x, y+i*30, w, h }, &innerRadii[i], 0.0f, 1.0f);
        resistanceTextBoxes[i].init(rgb[i], { x, y+i*30 ,w, h }, &resistances[i], 0.0f, 1.0f);
        x += 60;
    }
}

void UserInterface::update(Vector2 mousePos)
{
    // handle textbox submission
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
        if (activeTextBoxPtr != nullptr)
            activeTextBoxPtr->submit();

    // handle textbox selection
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        if (selectTextBox(mousePos, stepTextBox));
        else if (activeTextBoxPtr != nullptr)
            activeTextBoxPtr->active = false,
            activeTextBoxPtr->setText(),
            activeTextBoxPtr = nullptr;
}

void UserInterface::render()
{
    DrawRectangleRec(sideBarBounds, LIGHTGRAY);
    gridBoolCheckBox.render();
    stepTextBox.render();
}

bool UserInterface::selectTextBox(Vector2 mousePos, TextBox& textBox)
{
    if (CheckCollisionPointRec(mousePos, textBox.bounds)) {
        if (activeTextBoxPtr != nullptr)
            activeTextBoxPtr->active = false;
        textBox.active = true;
        activeTextBoxPtr = &textBox;
        return true;
    }
    return false;
}



void UserInterface::Label::init(const char text[BUFFER_LENGTH], Rectangle bounds)
{
    std::strcpy(this->text, text);
    this->bounds = bounds;
}



void UserInterface::Element::init(const char labelText[BUFFER_LENGTH], Rectangle bounds)
{
    label.init(labelText, { 0.0f, bounds.y, bounds.x-10.0f, 20.0f });
    this->bounds = bounds;
    active = false;    
}



void UserInterface::CheckBox::render()
{
    GuiDrawText(label.text, label.bounds, TEXT_ALIGN_RIGHT, DARKGRAY);
    GuiCheckBox(bounds, "", &active);
}


void UserInterface::TextBox::render()
{
    GuiDrawText(label.text, label.bounds, TEXT_ALIGN_RIGHT, (isValid()) ? DARKGRAY : RED);
    GuiTextBox(bounds, text, BUFFER_LENGTH, active);
}



void UserInterface::FloatTextBox::init(const char labelText[BUFFER_LENGTH], Rectangle bounds, float* valuePtr, float min, float max)
{
    TextBox::init(labelText, bounds);
    this->valuePtr = valuePtr;
    this->min = min;
    this->max = max;
    setText();
}

bool UserInterface::FloatTextBox::isValid()
{
    return std::regex_match(text, std::regex("^[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?$"));
}

void UserInterface::FloatTextBox::setText()
{
    snprintf(text, sizeof(text), "%f", *valuePtr);
}

void UserInterface::FloatTextBox::submit()
{
    if (isValid()) *valuePtr = std::atof(text);
}
