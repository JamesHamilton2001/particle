#pragma once

#include <raylib.h>
#include <iostream>
#include <cstdlib>
#include <regex>
#include <variant>

#include "raygui.h"


class UserInterface
{
    static const unsigned int BUFFER_LENGTH = 32u;

    class Label {
     public:
        void init(const char text[BUFFER_LENGTH], Rectangle bounds);
        Rectangle bounds;
        char text[16];
    };
    class Element {
     public:
        Rectangle bounds;
        Label label;
        bool active;
        void init(const char labelText[BUFFER_LENGTH], Rectangle bounds);
        virtual void render() = 0;
    };

    class CheckBox : public Element {
     public:
        void render();
    };
    class TextBox : public Element {
     public:
        char text[BUFFER_LENGTH];
        virtual bool isValid() = 0;
        virtual void setText() = 0;
        virtual void submit() = 0;
        void render();
    };
    class FloatTextBox : public TextBox {
     public:
        void init(const char labelText[BUFFER_LENGTH], Rectangle bounds, float* valuePtr, float min, float max);
        void setText();
        void submit();
     private:
        float min, max;
        float* valuePtr;
        bool isValid();
    };
    class FloatTextBoxSet {
     public:
        void init(const char labelText[BUFFER_LENGTH], FloatTextBox floatTextBoxes[3]);
        void render();
     private:
        Label label;
        FloatTextBox* floatTextBoxes;
    };

public:

    TextBox* activeTextBoxPtr = nullptr;

    UserInterface();

    void init(
        int windowWidth,
        int windowHeight,
        float* stepPtr,
        float innerRadii[3],
        float resistances[3],
        float attractions[3][3],
        Color colours[3]
    );
    void update(Vector2 mousePos);
    void render();
    bool selectTextBox(Vector2 mousePos, TextBox& textBox);
    bool gridOn();

private:

    Rectangle sideBarBounds;

    CheckBox gridBoolCheckBox;
    FloatTextBox stepTextBox;
    FloatTextBox innerRadiusTextBoxes[3];
    FloatTextBox resistanceTextBoxes[3];
    FloatTextBox attractionTextBoxes[3][3];

    FloatTextBoxSet innerRadiusSet;
    FloatTextBoxSet resistanceSet;
};