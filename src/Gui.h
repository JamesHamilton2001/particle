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

    struct Label   { Rectangle bounds; char text[LABEL_BUF_LEN]; };
    struct Element { Rectangle bounds; };
    
    struct CheckBox : Element { bool* valuePtr; };
    struct FloatBox : Element { float* valuePtr; int min; int max; bool editMode; };
    struct FloatSet { FloatBox floatBox1, floatBox2, floatBox3; };
    struct FloatMat { FloatSet floatSet1, floatSet2, floatSet3; };

    Label gridLabel;
    Label stepLabel;
    Label resistanceLabel;
    Label innerRadiiLabel;
    Label attractionsLabel;

    CheckBox gridCheckBox;
    FloatBox stepFloatBox;
    FloatBox resistanceFloatBox;
    FloatSet innerRadiiFloatSet;
    FloatMat attractionsMat;

    void initLabel(Label& label, Vector2 position, const char* labelText);
    void initCheckBox(CheckBox& checkBox, Vector2 position, bool* valuePtr);
    void initFloatBox(FloatBox& floatBox, Vector2 position, float* valuePtr, int min, int max);
    void initFloatSet(FloatSet& floatSet, Vector2 position, float valueSet[3], int min, int max);
    void initFloatMat(FloatMat& floatMat, Vector2 position, float valueMat[3][3], int min, int max);

    void handleLabel(Label& label);
    void handleCheckBox(CheckBox& checkBox);
    void handleFloatBox(FloatBox& floatBox);
    void handleFloatSet(FloatSet& floatSet);
    void handleFloatMat(FloatMat& floatMat);

};