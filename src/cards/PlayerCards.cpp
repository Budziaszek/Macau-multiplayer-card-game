//
// Created by Magdalena on 18.09.2020.
//

#include "PlayerCards.h"
#include "Deck.h"

PlayerCards::PlayerCards() {
    numberOfCards = 52;
    cards = new Card[numberOfCards];
    selectedCard = -1;
}

unsigned int PlayerCards::getNumberOfCards() const {
    return numberOfCards;
}

Card PlayerCards::getCard(unsigned int i) {
    return cards[i];
}

void PlayerCards::selectCard(unsigned int i) {
    cards[selectedCard].chosen = false;
    cards[i].chosen = true;
    selectedCard = (int) i;
}

void PlayerCards::resetCardSelection() {
    if (selectedCard != -1) {
        cards[selectedCard].chosen = false;
        selectedCard = -1;
    }
}

void PlayerCards::show() {
    for (int i = 0; i < numberOfCards; i++)
        cout << cards[i].color << " " << cards[i].figure << endl;
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
    if (selectedCard != -1) {
        cards[selectedCard].chosen = false;
        for (int i = selectedCard; i < numberOfCards - 1; i++)
            cards[i] = cards[i + 1];
        return cards[selectedCard];
    }
    return Card();
}

void PlayerCards::discard(unsigned int i, Deck &deck) {
    deck.getOne(cards[i]);
    cards[i].chosen = false;
    for (; i < numberOfCards - 1; i++)
        cards[i] = cards[i + 1];
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

