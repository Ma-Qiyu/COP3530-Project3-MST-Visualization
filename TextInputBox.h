#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

class TextInputBox {
public:
    RectangleShape box;
    Text text;
    string input;
    bool isActive = false;
    unsigned int limit = 6;
};

void initTextInputBox(TextInputBox& tbox, Font& font, Vector2f position, Vector2f size);

void handleTextInput(Event& event, TextInputBox& tbox);

void updateTextInputBox(TextInputBox& tbox, const RenderWindow& window);

void drawTextInputBox(RenderWindow& window, TextInputBox& tbox);
