//
// Created by Magdalena on 18.09.2020.
//

#ifndef MACAU_DECK_H
#define MACAU_DECK_H

#include <iostream>
#include "PlayerCards.h"

using namespace std;

class Deck {
public:
    Deck();

    void show();

    void shuffle(unsigned int times = 100);

    PlayerCards *deal(unsigned int numberOfPlayers, unsigned int numberOfCardsForEach);

    Card giveOne();

    void getOne(Card card);

private:
    Card *cards;
    unsigned int firstInDeck;
    unsigned int lastBackInDeck;

    void swapRandomCards();
};

#endif //MACAU_DECK_H
