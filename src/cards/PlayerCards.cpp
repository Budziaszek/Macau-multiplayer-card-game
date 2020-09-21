//
// Created by Magdalena on 18.09.2020.
//

#include "PlayerCards.h"
#include "Deck.h"

PlayerCards::PlayerCards() {
    cards = vector<Card>();
    selectedCard = -1;
}

unsigned int PlayerCards::getNumberOfCards() const {
    return cards.size();
}

Card PlayerCards::getCard(unsigned int i) {
    return cards[i];
}

void PlayerCards::selectCard(unsigned int i) {
    cards[selectedCard].selected = false;
    cards[i].selected = true;
    selectedCard = (int) i;
}

void PlayerCards::resetCardSelection() {
    if (selectedCard != -1) {
        cards[selectedCard].selected = false;
        selectedCard = -1;
    }
}

void PlayerCards::show() {
    for (auto &card : cards)
        cout << card.getColor() << " " << card.getFigure() << endl;
}

void PlayerCards::draw(Deck &deck) {
    cards.push_back(deck.getCard());
}

void PlayerCards::draw(Card card) {
    cards.push_back(card);
}

Card PlayerCards::discard() {
    Card card = Card();
    if (selectedCard != -1) {
        cards[selectedCard].selected = false;
        card = cards[selectedCard];
        cards.erase(cards.begin() + selectedCard);
    }
    return card;
}

void PlayerCards::discard(unsigned int i, Deck &deck) {
    deck.addCard(cards[i]);
    cards[i].selected = false;
    cards.erase(cards.begin() + selectedCard);
}

