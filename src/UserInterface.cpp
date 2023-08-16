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
    sideBarBounds = { 0, 0, 240, (float)(windowHeight) };
    char labels[3][1+1] = { "R", "G", "B" };
    float x, y, w, h;
    
    x = 60, y = 200, w = 20, h = 20;
    gridBoolCheckBox.init("Grid", { x, y, w, h });

    y += 30, w = 100;
    stepTextBox.init("Step", { x, y, w, h }, stepPtr, 0.0, 10000.0f);

    y += 60; w = 30;
    for (int i = 0; i < 3; i++) {
        innerRadiusTextBoxes[i].init(labels[i], { x, y, w, h }, &innerRadii[i], 0.0f, 1.0f);
        resistanceTextBoxes[i].init(labels[i], { x, y + 60, w, h }, &resistances[i], 0.0f, 1.0f);
        x += 60;
    }
    innerRadiusSet.init("Inner Radii Set", innerRadiusTextBoxes);
    resistanceSet.init("Resistance Set", resistanceTextBoxes);
}

void UserInterface::update(Vector2 mousePos)
{
    // handle textbox submission
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
        if (activeTextBoxPtr != nullptr)
            activeTextBoxPtr->submit();

    // handle textbox selection
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        bool selected = selectTextBox(mousePos, stepTextBox);
        for (int i = 0; i < 3; i++) {
            if (selected) break;
            selected =  selectTextBox(mousePos, innerRadiusTextBoxes[i]) ||
                        selectTextBox(mousePos, resistanceTextBoxes[i]) || (
                            selectTextBox(mousePos, attractionTextBoxes[i][0]) ||
                            selectTextBox(mousePos, attractionTextBoxes[i][1]) ||
                            selectTextBox(mousePos, attractionTextBoxes[i][2]) );
        }
        if (!selected && activeTextBoxPtr != nullptr)
            activeTextBoxPtr->active = false,
            activeTextBoxPtr->setText(),
            activeTextBoxPtr = nullptr;
    }
}

void UserInterface::render()
{
    DrawRectangleRec(sideBarBounds, LIGHTGRAY);
    gridBoolCheckBox.render();
    stepTextBox.render();
    innerRadiusSet.render();
    resistanceSet.render();
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

bool UserInterface::gridOn()
{
    return gridBoolCheckBox.active;
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



void UserInterface::FloatTextBoxSet::init(const char labelText[BUFFER_LENGTH], FloatTextBox floatTextBoxes[3])
{
    this->floatTextBoxes = floatTextBoxes;
    float x = floatTextBoxes[0].bounds.x;
    float y = floatTextBoxes[0].bounds.y - floatTextBoxes[0].bounds.height;
    float w = floatTextBoxes[2].bounds.x + floatTextBoxes[2].bounds.width - floatTextBoxes[0].bounds.x;
    float h = 20.0f;
    label.init(labelText, { x, y, w, h });
}

void UserInterface::FloatTextBoxSet::render()
{
    GuiDrawText(label.text, label.bounds, TEXT_ALIGN_LEFT, DARKGRAY);
    for (int i = 0; i < 3; i++) floatTextBoxes[i].render();
}