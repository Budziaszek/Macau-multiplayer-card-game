//
// Created by Magdalena on 22.09.2020.
//

#ifndef MACAU_BUTTON_H
#define MACAU_BUTTON_H

#include <SFML/Graphics.hpp>

using namespace std;

class Button : public sf::Drawable {
public:
    Button();

    Button(const string &str, const sf::Font &font, unsigned int size);

    bool checkMouseEvent(sf::Vector2f mousePosition, sf::Event event);

    void setColors(sf::Color basic = sf::Color::White, sf::Color cover = sf::Color::Yellow);

    void setPosition(float x, float y);

    void move(float x, float y);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Text* getText();

private:
    sf::Text text;
    sf::Color basicColor;
    sf::Color coverColor;
};

#endif //MACAU_BUTTON_H
