//
// Created by Magdalena on 18.09.2020.
//

#include "Card.h"

Card::Card(unsigned int color, unsigned int figure) {
    Card::figure = check(figure, 0, 13);
    Card::color = check(color, 0, 4);
    selected = false;
}

unsigned int Card::getColor() const {
    return color;
}

unsigned int Card::getFigure() const {
    return figure;
}

bool Card::isSpecial() const {
    switch (figure) {
        case Two:
        case Three:
        case Four:
        case Jack:
        case Ace:
            return true;
        case King:
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

void Card::changeSelection() {
    selected = !selected;
}
