//
// Created by Magdalena on 18.09.2020.
//

#include "PlayerCards.h"

PlayerCards::PlayerCards() {
    numberOfCards = 0;
    cards = new Card[52];
}

Card PlayerCards::checkOne(int which) {
    return cards[which];
}

void PlayerCards::show() {
    for (int i = 0; i < numberOfCards; i++) {
        cout << cards[i].color << " " << cards[i].figure << endl;
    }
}

void PlayerCards::draw(Deck &deck) {
    cards[numberOfCards] = deck.giveOne();
    numberOfCards++;
}

void PlayerCards::draw(Card card) {
    cards[numberOfCards] = card;
    numberOfCards++;
}

Card PlayerCards::discard() {
    for (int i = 0; i < numberOfCards; i++) {
        if (cards[i].chosen == true) {
            Card thisOne = cards[i];
            for (; i < numberOfCards - 1; i++) {
                cards[i] = cards[i + 1];
            }
            numberOfCards--;
            cards[i].chosen = false;
            return thisOne;
        }
    }
    return Card();
}

void PlayerCards::discard(unsigned int which, Deck &deck) {
    deck.getOne(cards[which]);
    cards[which].chosen = false;
    for (int i = which; i < numberOfCards - 1; i++) {
        cards[i] = cards[i + 1];
    }
    numberOfCards--;
}

void PlayerCards::discard(Card card, Deck &deck) {
    deck.getOne(card);
    for (int i = 0; i < numberOfCards; i++) {
        if (cards[i].color == card.color)
            if (cards[i].figure == card.figure) {
                cards[i].chosen = true;
                discard();
                return;
            }
    }
}

void PlayerCards::changeChoose(int which) {
    cards[which].chosen = !cards[which].chosen;
}

void PlayerCards::resetChoosen() {
    for (int i = 0; i < numberOfCards; i++)
        cards[i].chosen = false;
}
