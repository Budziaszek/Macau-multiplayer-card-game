//
// Created by Magdalena on 18.09.2020.
//

#ifndef MACAU_PLAYERCARDS_H
#define MACAU_PLAYERCARDS_H

#include <iostream>
#include "Card.h"

using namespace std;

class Deck;

class PlayerCards {
    friend class Deck;

public:
    PlayerCards();

    unsigned int getNumberOfCards() const;

    Card getCard(unsigned int i);

    void selectCard(unsigned int i);

    void resetCardSelection();

    void show();

    void draw(Deck &deck);

    void draw(Card card);

    Card discard();

    void discard(unsigned int i, Deck &deck);

    void discard(Card card, Deck &deck);

private:
    Card *cards;
    unsigned int numberOfCards;
    int selectedCard;
};

#endif //MACAU_PLAYERCARDS_H
