//
// Created by Magdalena on 18.09.2020.
//

#include "CardsImages.h"

bool CardsImages::load(const std::string &tileset, sf::Vector2u tileSize) {
    if (!Tileset.loadFromFile(tileset))
        return false;

    vertex = new sf::VertexArray[52];
    size = tileSize;

    for (unsigned int tileNumber = 0; tileNumber < 52; tileNumber++) {
        vertex[tileNumber].setPrimitiveType(sf::Quads);
        vertex[tileNumber].resize(4);
        sf::Vertex *quad = &(vertex[tileNumber][0]);

        int n = tileNumber % (Tileset.getSize().x / tileSize.x);
        int m = tileNumber / (Tileset.getSize().x / tileSize.x);

        quad[0].texCoords = sf::Vector2f(n * tileSize.x, m * tileSize.y);
        quad[1].texCoords = sf::Vector2f((n + 1) * tileSize.x, m * tileSize.y);
        quad[2].texCoords = sf::Vector2f((n + 1) * tileSize.x, (m + 1) * tileSize.y);
        quad[3].texCoords = sf::Vector2f(n * tileSize.x, (m + 1) * tileSize.y);

        quad[0].position = sf::Vector2f(0, 0);
        quad[1].position = sf::Vector2f(tileSize.x, 0);
        quad[2].position = sf::Vector2f(tileSize.x, tileSize.y);
        quad[3].position = sf::Vector2f(0, tileSize.y);
    }
    return true;
}

void CardsImages::setNumber(int cardNumber) {
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
    states.texture = &Tileset;
    target.draw(vertex[number], states);
}