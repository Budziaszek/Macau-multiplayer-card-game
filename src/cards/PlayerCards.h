//
// Created by Magdalena on 18.09.2020.
//

#ifndef MACAU_PLAYERCARDS_H
#define MACAU_PLAYERCARDS_H

#include <iostream>
#include <vector>
#include "Card.h"

using namespace std;

class Deck;

class PlayerCards {
public:
    PlayerCards();

    unsigned int getNumberOfCards() const;

    Card getCard(unsigned int i);

    void changeCardSelection(unsigned int i);

    void resetCardSelection();

    void show();

    void draw(Deck &deck);

    void draw(Card card);

    Card discard();

    void discard(unsigned int i, Deck &deck);

private:
    vector<Card> cards;
    int selectedCard;
};

#endif //MACAU_PLAYERCARDS_H
