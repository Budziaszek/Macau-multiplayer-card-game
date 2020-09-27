//
// Created by Magdalena on 18.09.2020.
//

#ifndef MACAU_CARD_H
#define MACAU_CARD_H

class Card {
public:
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

    bool isSpecial() const;

    bool isSelected() const;

    void setSelected(bool s);

    void changeSelection();

private:
    unsigned int color;
    unsigned int figure;
    bool selected;

private:

    static unsigned int check(unsigned int value, unsigned int min, unsigned int max) {
        if (value > max)
            return max;
        if (value < min)
            return min;
        return value;
    }
};

#endif //MACAU_CARD_H
