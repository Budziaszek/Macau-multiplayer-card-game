//
// Created by Magdalena on 18.09.2020.
//

#ifndef MACAU_CARDSIMAGES_H
#define MACAU_CARDSIMAGES_H

#include <SFML/Graphics.hpp>
#include "../cards/Card.h"

class CardsImages : public sf::Drawable, public sf::Transformable {
public:
    bool load(const std::string &tileset, sf::Vector2u tileSize);

    void setNumber(unsigned int cardNumber);

    void setCard(Card card);

    void setPosition(sf::Vector2i point);

    sf::Vector2u getSize();

    sf::RectangleShape *getCardBack();

    void initializePositions(int windowWidth, int windowHeight);

    float getXPosition(int player);

    float getYPosition(int player);

    float getRotation(int player);

    float getXStep(int player);

    float getYStep(int player);

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    unsigned int number;
    sf::Vector2u size;
    sf::VertexArray *vertex;
    sf::Texture texture;
    sf::RectangleShape cardBack;

    float *xPosition;
    float *yPosition;
    float *rotation;
    float *xStep;
    float *yStep;
};

#endif //MACAU_CARDSIMAGES_H
