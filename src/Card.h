//
// Created by Magdalena on 18.09.2020.
//

#ifndef MACAU_CARD_H
#define MACAU_CARD_H

class Card {
    friend class Deck;

    friend class PlayerCards;

public:
    bool chosen;
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

private:
    unsigned int color;
    unsigned int figure;

public:
    explicit Card(unsigned int givenColour = noColor, unsigned int givenFigure = noFigure);

    unsigned int getColor() const;

    unsigned int getFigure() const;
};

#endif //MACAU_CARD_H
