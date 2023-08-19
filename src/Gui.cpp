#include "Gui.h"

#include <raylib.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// NOTE: float box is taken and customised from custom input box example
// changes made: handle 8 decimal places, added left padding



int guiFloatingPointIndex = 0;                                                                                // Global variable shared by all GuiFLoatBox()

float TextToFloat(const char* text);                                                                          // Helper function that converts text to float
int GuiFloatBox(Rectangle bounds, const char* text, float* value, int minValue, int maxValue, bool editMode); // Custom input box that works with float values. Basicly GuiValueBox(), but with some changes



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
    Rectangle bounds;

    // grid check box
    bounds = { 100, windowHeight - 250, 20, 20 };
    gridCheckBox.bounds = bounds;
    gridCheckBox.valuePtr = &canvas.drawGrid;
    gridLabel = elementLabel(bounds, "DrawGrid", TEXT_ALIGN_RIGHT);

    // step float box
    bounds.y += 30; bounds.width = 100;
    stepFloatBox.bounds = bounds;
    stepFloatBox.valuePtr = &particleLife.step;
    stepFloatBox.min = 0;
    stepFloatBox.max = 1;
    stepFloatBox.editMode = false;
    stepLabel = elementLabel(bounds, "Step", TEXT_ALIGN_RIGHT);
}

void Gui::updateRender()
{
    handleLabel(gridLabel);
    handleCheckBox(gridCheckBox);

    handleLabel(stepLabel);
    handleFloatBox(stepFloatBox);
}

Gui::Label Gui::elementLabel(Rectangle elementBounds, const char labelText[LABEL_BUF_LEN], GuiTextAlignment alignment)
{
    Label label = { { elementBounds.x - 70, elementBounds.y, 60, 20 }, "", alignment };
    std::strcpy(label.text, labelText);
    return label;
}

void Gui::handleLabel(Label& label)
{
    GuiDrawText(label.text, label.bounds, label.alignment, WHITE);
}

void Gui::handleCheckBox(CheckBox& checkBox)
{
    GuiCheckBox(checkBox.bounds, NULL, checkBox.valuePtr);
}

void Gui::handleFloatBox(FloatBox& floatBox)
{
    if (GuiFloatBox(floatBox.bounds, NULL, floatBox.valuePtr, floatBox.min, floatBox.max, floatBox.editMode))
        floatBox.editMode = !floatBox.editMode;
}









// Get float value from text
float TextToFloat(const char* text)
{
	float value = 0.0f;
	float floatingPoint = 0.0f;
	int sign = 1;

    // deal with the sign
	if ((text[0] == '+') || (text[0] == '-')) {
		if (text[0] == '-') sign = -1;
		text++;
	}

    // convert text to float
	for (int i = 0; (((text[i] >= '0') && (text[i] <= '9')) || text[i] == '.'); i++) {
		if (text[i] == '.') {
			if (floatingPoint > 0.0f) break;
			floatingPoint = 10.0f;
			continue;
		}
		if (floatingPoint > 0.0f)  // after encountering decimal separator
			value += (float)(text[i] - '0') / floatingPoint,
			floatingPoint *= 10.0f;
		
		else value = value * 10.0f + (float)(text[i] - '0'); // before decimal separator 
	}
	return value * sign;
}

// Float Box control, updates input text with numbers
int GuiFloatBox(Rectangle bounds, const char* text, float* value, int minValue, int maxValue, bool editMode)
{
#if !defined(RAYGUI_VALUEBOX_MAX_CHARS)
#define RAYGUI_VALUEBOX_MAX_CHARS  32
#endif

    int result = 0;
    GuiState state = guiState;

    char textValue[RAYGUI_VALUEBOX_MAX_CHARS + 1] = "\0";

    Rectangle textBounds = { 0 };
    if (text != NULL)
    {
        textBounds.width = (float)GetTextWidth(text) + 2;
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x + bounds.width + GuiGetStyle(VALUEBOX, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height / 2 - GuiGetStyle(DEFAULT, TEXT_SIZE) / 2;
        if (GuiGetStyle(VALUEBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT) textBounds.x = bounds.x - textBounds.width - GuiGetStyle(VALUEBOX, TEXT_PADDING);
    }

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiSliderDragging) {

        Vector2 mousePoint = GetMousePosition();

        if (*value >= 0) sprintf(textValue, "+%.8f", *value);
        else sprintf(textValue, "%.8f", *value);

        bool valueHasChanged = false;

        int keyCount = (int)strlen(textValue) - guiFloatingPointIndex;

        if (editMode)
        {
            state = STATE_PRESSED;

            // Only allow keys in range [48..57]
            if (keyCount < RAYGUI_VALUEBOX_MAX_CHARS)
            {
                if (GetTextWidth(textValue) < bounds.width)
                {
                    int key = GetCharPressed();
                    if ((key >= 48) && (key <= 57) && guiFloatingPointIndex)
                    {
                        if (guiFloatingPointIndex && guiFloatingPointIndex != 9) guiFloatingPointIndex--;

                        textValue[keyCount] = (char)key;
                        textValue[++keyCount] = '\0';
                        valueHasChanged = true;
                    }
                }
            }

            // Delete text
            if (keyCount > 0)
            {
                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    if (guiFloatingPointIndex < 9) guiFloatingPointIndex++;

                    keyCount--;
                    textValue[keyCount] = '\0';
                    valueHasChanged = true;
                }
            }

            // Change sign
            if (IsKeyPressed(KEY_MINUS))
            {
                if (textValue[0] == '+') textValue[0] = '-';
                else if (textValue[0] == '-') textValue[0] = '+';
                valueHasChanged = true;
            }

            // Add decimal separator
            if ((IsKeyPressed(KEY_COMMA) || IsKeyPressed(KEY_PERIOD)) && guiFloatingPointIndex == 9)
            {
                guiFloatingPointIndex--;
                valueHasChanged = true;
            }

            if (valueHasChanged)
            {
                *value = TextToFloat(textValue);
            }

            if (IsKeyPressed(KEY_ENTER) || (!CheckCollisionPointRec(mousePoint, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
            {
                guiFloatingPointIndex = 0;
                result = 1;
            }
        }
        else
        {
            if (*value > maxValue) *value = maxValue;
            else if (*value < minValue) *value = minValue;

            if (CheckCollisionPointRec(mousePoint, bounds))
            {
                state = STATE_FOCUSED;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) result = 1;
            }
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    Color baseColor = BLANK;
    sprintf(textValue, "%.8f", *value);

    if (state == STATE_PRESSED)
    {
        baseColor = GetColor(GuiGetStyle(VALUEBOX, BASE_COLOR_PRESSED));
        textValue[(int)strlen(textValue) - guiFloatingPointIndex] = '\0';
    }
    else if (state == STATE_DISABLED) baseColor = GetColor(GuiGetStyle(VALUEBOX, BASE_COLOR_DISABLED));

    // WARNING: BLANK color does not work properly with Fade()
    GuiDrawRectangle(bounds, GuiGetStyle(VALUEBOX, BORDER_WIDTH), Fade(GetColor(GuiGetStyle(VALUEBOX, BORDER + (state * 3))), guiAlpha), baseColor);
    GuiDrawText(textValue, GetTextBounds(VALUEBOX, bounds), TEXT_ALIGN_CENTER, Fade(GetColor(GuiGetStyle(VALUEBOX, TEXT + (state * 3))), guiAlpha));

    // Draw cursor
    if (editMode)
    {
        // NOTE: ValueBox internal text is always centered
        Rectangle cursor = { bounds.x + GetTextWidth(textValue) / 2 + bounds.width / 2 + 1, bounds.y + 2 * GuiGetStyle(VALUEBOX, BORDER_WIDTH), 4, bounds.height - 4 * GuiGetStyle(VALUEBOX, BORDER_WIDTH) };
        GuiDrawRectangle(cursor, 0, BLANK, Fade(GetColor(GuiGetStyle(VALUEBOX, BORDER_COLOR_PRESSED)), guiAlpha));
    }

    // Draw text label if provided
    GuiDrawText(text, textBounds, (GuiGetStyle(VALUEBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT) ? TEXT_ALIGN_LEFT : TEXT_ALIGN_RIGHT, Fade(GetColor(GuiGetStyle(LABEL, TEXT + (state * 3))), guiAlpha));
    //--------------------------------------------------------------------

    return result;
}