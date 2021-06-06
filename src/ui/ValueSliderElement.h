#pragma once

#include "UIContainer.h"
#include "TextElement.h"
#include "SliderBarElement.h"

#include "utils/Rectangle.h"

class ValueSliderElement : public UIContainer {
public:
    ValueSliderElement(vec2 parentPos, vec2 parentSize, std::string valueName, float* valueToChange);
    void handle_mouse() override;
    void click();
private:
    TextElement* name;
    SliderBarElement* slider;
    TextElement* valueDisplay;
    float minValue, maxValue;
    float* value;

};