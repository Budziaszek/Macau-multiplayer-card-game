//
// Created by Magdalena on 18.09.2020.
//

#include "Card.h"

Card::Card(unsigned int givenColor, unsigned int givenFigure) {
    if (givenFigure > 13)
        givenFigure = 13;
    if (givenFigure < 0)
        givenFigure = 0;
    figure = givenFigure;

    if (givenColor > 4)
        givenColor = 4;
    if (givenColor < 0)
        givenColor = 0;
    color = givenColor;
    chosen = false;
}

unsigned int Card::getColor() const {
    return color;
}

unsigned int Card::getFigure() const {
    return figure;
}
