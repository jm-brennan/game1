#include "ValueSliderElement.h"


ValueSliderElement::ValueSliderElement(vec2 parentPos, vec2 parentSize, std::string valueName, float* valueToChange) {
    pos = parentPos;
    size = parentSize;

    value = valueToChange;
    minValue = *value - *value;
    maxValue = *value + *value;
    
    name = new TextElement(
        pos, 
        size, 
        valueName
    );
    add_child(name);
    
    slider = new SliderBarElement(
        vec2(pos.x, pos.y + name->size.y), 
        vec2(size.x, size.y - name->size.y) // @TODO @CLEANUP only uses x value
    );
    add_child(slider);
    
    valueDisplay = new TextElement(
        vec2(pos.x, pos.y + name->size.y + slider->size.y), 
        size, // @TODO @CLEANUP only uses the x right now, cleanup after implementing alignments?
        std::to_string(*value)
    );
    add_child(valueDisplay);
}

void ValueSliderElement::handle_mouse() {
    UIContainer::handle_mouse();
    *value = slider->get_fraction() * (maxValue - minValue);
    valueDisplay->set_text(std::to_string(*value));

}
