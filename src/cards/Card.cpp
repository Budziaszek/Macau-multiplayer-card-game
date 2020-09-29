//
// Created by Magdalena on 18.09.2020.
//

#include "Card.h"

Card::Card(unsigned int color, unsigned int figure) {
    Card::figure = check(figure, 0, 13);
    Card::color = check(color, 0, 4);
    selected = false;
}

string Card::colorToString(int color) {
    switch (color) {
        case club:
            return "club";
        case heart:
            return "heart";
        case spade:
            return "spade";
        case diamond:
            return "diamond";
        default:
            return "null";
    }
}

string Card::figureToString(int figure) {
    switch (figure) {
        case ace:
            return "Ace";
        case two:
            return "Two";
        case three:
            return "Three";
        case four:
            return "Four";
        case five:
            return "Five";
        case six:
            return "Six";
        case seven:
            return "Seven";
        case eight:
            return "Eight";
        case nine:
            return "Nine";
        case ten:
            return "Ten";
        case jack:
            return "Jack";
        case queen:
            return "Queen";
        case king:
            return "King";
        default:
            return "null";
    }
}

unsigned int Card::getColor() const {
    return color;
}

unsigned int Card::getFigure() const {
    return figure;
}

bool Card::isBrave() const{
    switch (figure) {
        case two:
        case three:
            return true;
        case king:
            if (color == spade || color == heart)
                return true;
        default:
            return false;
    }
}

bool Card::isSpecial() const {
    switch (figure) {
        case two:
        case three:
        case four:
        case jack:
        case ace:
            return true;
        case king:
            if (color == spade || color == heart)
                return true;
        default:
            return false;
    }
}

bool Card::isSelected() const {
    return selected;
}

void Card::setSelected(bool s) {
    Card::selected = s;
}

void Card::setColor(unsigned int c) {
    Card::color = c;
}

void Card::setFigure(unsigned int f) {
    Card::figure = f;
}

unsigned int Card::getPower() const {
    switch (figure) {
        case two:
            return 2;
        case three:
            return 3;
        case king:
            if (color == spade || color == heart)
                return 5;
        default:
            return 0;
    }
}

unsigned int Card::check(unsigned int value, unsigned int min, unsigned int max) {
    if (value > max)
        return max;
    if (value < min)
        return min;
    return value;
}

bool Card::isFullyDefined() {
    return getFigure() != noFigure && getColor() != noColor;
}

