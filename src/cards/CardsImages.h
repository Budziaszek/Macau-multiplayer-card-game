//
// Created by Magdalena on 18.09.2020.
//

#ifndef MACAU_CARDSIMAGES_H
#define MACAU_CARDSIMAGES_H

#include <SFML/Graphics.hpp>

class CardsImages : public sf::Drawable, public sf::Transformable {
public:
    bool load(const std::string &tileset, sf::Vector2u tileSize);

    void setNumber(int cardNumber);

    void setPosition(sf::Vector2i point);

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    int number;
    sf::Vector2u size;
    sf::VertexArray *vertex;
    sf::Texture Tileset;
};

#endif //MACAU_CARDSIMAGES_H
