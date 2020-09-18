//
// Created by Magdalena on 18.09.2020.
//

#ifndef MACAU_PLAYERCARDS_H
#define MACAU_PLAYERCARDS_H

#include "Card.h"
#include "Deck.h"

class PlayerCards
{
    friend class Deck;
public:
    PlayerCards();
    Card checkOne(int which);
    void changeChoose(int which);
    void resetChoosen();
    unsigned int numberOfCards;
    void show();
    void draw(Deck &deck);
    void draw(Card card);
    Card discard();
    void discard(unsigned int which, Deck& deck);
    void discard(Card card, Deck& deck);
private:
    Card* cards;

};


#endif //MACAU_PLAYERCARDS_H
