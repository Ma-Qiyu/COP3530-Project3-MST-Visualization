#include "TextInputBox.h"

void initTextInputBox(TextInputBox& tbox, Font& font, Vector2f position, Vector2f size) {
    tbox.box.setSize(size);
    tbox.box.setPosition(position);
    tbox.box.setFillColor(Color(30, 30, 30));
    tbox.box.setOutlineThickness(2.f);
    tbox.box.setOutlineColor(Color(80, 80, 80));

    tbox.text.setFont(font);
    tbox.text.setCharacterSize(size.y * 0.7);
    tbox.text.setFillColor(Color::White);
    tbox.text.setPosition(position.x + 10, position.y + (size.y * 0.1));
    tbox.input = "200"; // Default
    tbox.text.setString(tbox.input);
}

void handleTextInput(Event& event, TextInputBox& tbox) {
    if (tbox.isActive && event.type == Event::TextEntered) {
        // Check ASCII code
        if (event.text.unicode < 128) {
            char typedChar = static_cast<char>(event.text.unicode);
            // If is a backspace
            if (typedChar == '\b') {
                if (!tbox.input.empty()) {
                    tbox.input.pop_back();
                }
            }
            // If is digit and not exceed length limit
            else if (isdigit(typedChar) && tbox.input.length() < tbox.limit) {
                tbox.input += typedChar;
            }
        }
    }
}

void updateTextInputBox(TextInputBox& tbox, const RenderWindow& window) {
    tbox.text.setString(tbox.input);
    // Check if activated
    if (tbox.isActive) {
        tbox.box.setOutlineColor(Color::White);
    } else {
        tbox.box.setOutlineColor(Color(80, 80, 80));
    }
    // Update text
    tbox.text.setString(tbox.input);
}

void drawTextInputBox(RenderWindow& window, TextInputBox& tbox) {
    window.draw(tbox.box);
    window.draw(tbox.text);
}


