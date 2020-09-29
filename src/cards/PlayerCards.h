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

    Card getSelectedCard();

    void changeCardSelection(unsigned int i);

    void resetCardSelection();

    void show();

    void draw(Deck *deck);

    void draw(Card card);

    Card discard();

private:
    vector<Card> cards;
    int selectedCard;
};

#endif //MACAU_PLAYERCARDS_H
