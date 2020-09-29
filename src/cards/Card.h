//
// Created by Magdalena on 18.09.2020.
//

#ifndef MACAU_CARD_H
#define MACAU_CARD_H

#include <string>

using namespace std;

class Card {
public:
    enum Color {
        noColor = 0,
        club = 1,
        heart,
        spade,
        diamond
    };

    enum Figure {
        noFigure = 0,
        ace = 1,
        two,
        three,
        four,
        five,
        six,
        seven,
        eight,
        nine,
        ten,
        jack,
        queen,
        king,
    };

    static string colorToString(int color);

    static string figureToString(int figure);

    explicit Card(unsigned int color = noColor, unsigned int figure = noFigure);

    unsigned int getColor() const;

    unsigned int getFigure() const;

    bool isSpecial() const;

    bool isBrave() const;

    unsigned int getPower() const;

    bool isSelected() const;

    void setSelected(bool s);

    void setColor(unsigned int c);

    void setFigure(unsigned int f);

    bool isFullyDefined();

private:
    unsigned int color;
    unsigned int figure;
    bool selected;

    static unsigned int check(unsigned int value, unsigned int min, unsigned int max);
};

#endif //MACAU_CARD_H
