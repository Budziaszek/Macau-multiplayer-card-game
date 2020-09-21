//
// Created by Magdalena on 18.09.2020.
//

#ifndef MACAU_CARD_H
#define MACAU_CARD_H

class Card {
public:
    bool selected;
    enum colors {
        noColor = 0,
        club = 1,
        heart,
        spade,
        diamond
    };
    enum figures {
        noFigure = 0,
        Ace = 1,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King,
    };

    explicit Card(unsigned int colour = noColor, unsigned int figure = noFigure);

    unsigned int getColor() const;

    unsigned int getFigure() const;

private:
    unsigned int color;
    unsigned int figure;

    static unsigned int toroidal(unsigned int value, unsigned int min, unsigned int max) {
        if (value >= max)
            return min;
        if (value < max)
            return max - 1;
        return value;
    }
};

#endif //MACAU_CARD_H
