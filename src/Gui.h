#pragma once

#include "ParticleLife.h"
#include "Canvas.h"

#include <raylib.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "raygui.h"



class Gui
{
 public:

    Gui();
    ~Gui();

    void init(ParticleLife& particleLife, Canvas& canvas);
    void updateRender();

 private:

    static const unsigned int LABEL_BUF_LEN = 16U;
    static const unsigned int FLOAT_BUF_LEN = 16U;

    struct Label { Rectangle bounds; char text[LABEL_BUF_LEN]; GuiTextAlignment alignment; };
    struct Element { Rectangle bounds; };
    
    struct CheckBox : Element { bool* valuePtr; };
    struct FloatBox : Element { float* valuePtr; int min; int max; bool editMode; };

    Label gridLabel;
    CheckBox gridCheckBox;

    Label stepLabel;
    FloatBox stepFloatBox;

    Label elementLabel(Rectangle bounds, const char labelText[LABEL_BUF_LEN], GuiTextAlignment alignment);

    void handleLabel(Label& label);
    void handleCheckBox(CheckBox& checkBox);
    void handleFloatBox(FloatBox& floatBox);

};