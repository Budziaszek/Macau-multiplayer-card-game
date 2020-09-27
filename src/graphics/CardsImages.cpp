//
// Created by Magdalena on 18.09.2020.
//

#include "CardsImages.h"

bool CardsImages::load(const std::string &tileset, sf::Vector2u tileSize) {
    if (!texture.loadFromFile(tileset))
        return false;

    vertex = new sf::VertexArray[52];
    size = tileSize;

    for (unsigned int tileNumber = 0; tileNumber < 52; tileNumber++) {
        vertex[tileNumber].setPrimitiveType(sf::Quads);
        vertex[tileNumber].resize(4);
        sf::Vertex *quad = &(vertex[tileNumber][0]);

        unsigned int n = tileNumber % (texture.getSize().x / tileSize.x);
        unsigned int m = tileNumber / (texture.getSize().x / tileSize.x);

        quad[0].texCoords = sf::Vector2f(n * tileSize.x, m * tileSize.y);
        quad[1].texCoords = sf::Vector2f((n + 1) * tileSize.x, m * tileSize.y);
        quad[2].texCoords = sf::Vector2f((n + 1) * tileSize.x, (m + 1) * tileSize.y);
        quad[3].texCoords = sf::Vector2f(n * tileSize.x, (m + 1) * tileSize.y);

        quad[0].position = sf::Vector2f(0, 0);
        quad[1].position = sf::Vector2f(tileSize.x, 0);
        quad[2].position = sf::Vector2f(tileSize.x, tileSize.y);
        quad[3].position = sf::Vector2f(0, tileSize.y);
    }
    cardBack = sf::RectangleShape(sf::Vector2f(71, 96));
    cardBack.setFillColor(sf::Color(153, 0, 0, 255));
    cardBack.setOutlineThickness(-1);
    cardBack.setOutlineColor(sf::Color::Black);

    return true;
}

void CardsImages::setNumber(unsigned int cardNumber) {
    number = cardNumber;
}

void CardsImages::setPosition(sf::Vector2i point) {
    sf::Vertex *quad = &(vertex[number][0]);

    quad[0].position = sf::Vector2f(point.x, point.y);
    quad[1].position = sf::Vector2f(point.x + size.x, point.y);
    quad[2].position = sf::Vector2f(point.x + size.x, point.y + size.y);
    quad[3].position = sf::Vector2f(point.x, point.y + size.y);
}

void CardsImages::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &texture;
    target.draw(vertex[number], states);
}

sf::Vector2u CardsImages::getSize() {
    return size;
}

sf::RectangleShape *CardsImages::getCardBack() {
    return &cardBack;
}

void CardsImages::initializePositions(int windowWidth, int windowHeight) {
    float m = 170;
    xPosition = new float[4]{
            (float) windowWidth / 2.0f - (float) getSize().x / 2,
            (float) windowWidth / 2.0f - m,
            (float) windowWidth / 2.0f - (float) getSize().x / 2,
            (float) windowWidth / 2.0f + m + (float) getSize().x
    };
    yPosition = new float[4]{
            (float) windowHeight / 2.0f + m,
            (float) windowHeight / 2.0f - (float) getSize().x / 2,
            (float) windowHeight / 2.0f - m - (float) getSize().y,
            (float) windowHeight / 2.0f - (float) getSize().x / 2
    };
    rotation = new float[4]{0, 90, 0, 90};
    xStep = new float[4]{1, 0, 1, 0};
    yStep = new float[4]{0, 1, 0, 1};
}

float CardsImages::getYPosition(int player) {
    return yPosition[player];
}

float CardsImages::getRotation(int player) {
    return rotation[player];
}

float CardsImages::getXPosition(int player) {
    return xPosition[player];
}

void CardsImages::setCard(Card card) {
    setNumber((card.getColor() - 1) * 13 + card.getFigure() - 1);
}

float CardsImages::getXStep(int player) {
    return xStep[player];
}

float CardsImages::getYStep(int player) {
    return yStep[player];
}


