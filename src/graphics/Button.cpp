//
// Created by Magdalena on 22.09.2020.
//

#include <iostream>
#include "Button.h"

Button::Button() {
    text = sf::Text();
    text.setFillColor(basicColor);
}

Button::Button(const string& str, const sf::Font& font, unsigned int size) {
    setColors();
    text = sf::Text(str, font, size);
    text.setFillColor(basicColor);
}

bool Button::checkMouseEvent(sf::Vector2f mousePosition, sf::Event event) {
    if (text.getGlobalBounds().contains(mousePosition)) {
        text.setFillColor(coverColor);
        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            return true;
    }
    else
        text.setFillColor(basicColor);
    return false;
}

void Button::setColors(sf::Color basic, sf::Color cover) {
    basicColor = basic;
    coverColor = cover;
}

void Button::setPosition(float x, float y) {
    text.setPosition(x, y);
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(text);
}

sf::Text *Button::getText() {
    return &text;
}

void Button::move(float x, float y) {
    text.move(x, y);
}

