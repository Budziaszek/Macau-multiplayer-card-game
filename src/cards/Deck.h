//
// Created by Magdalena on 18.09.2020.
//

#ifndef MACAU_DECK_H
#define MACAU_DECK_H

#include <iostream>
#include <vector>
#include <random>
#include<algorithm>

using namespace std;

class Card;

class PlayerCards;

class Deck {
public:
    Deck();

    void show();

    void shuffle(unsigned int times = 100);

    PlayerCards *deal(unsigned int numberOfPlayers, unsigned int numberOfCardsForEach);

    Card getCard();

    void addCard(Card card);

private:
    vector<Card> cards;
    vector<Card> usedCards;
};

#endif //MACAU_DECK_H
