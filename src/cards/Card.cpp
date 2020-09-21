//
// Created by Magdalena on 18.09.2020.
//

#include "Card.h"

Card::Card(unsigned int color, unsigned int figure) {
    Card::figure = toroidal(figure, 0, 13);
    Card::color = toroidal(color, 0, 4);
    selected = false;
}

unsigned int Card::getColor() const {
    return color;
}

unsigned int Card::getFigure() const {
    return figure;
}
