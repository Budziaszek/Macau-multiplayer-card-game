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

void PlayerCards::changeCardSelection(unsigned int i) {
    if (cards[i].isSelected()) {
        cards[i].setSelected(false);
        selectedCard = -1;
    } else {
        resetCardSelection();
        cards[i].setSelected(true);
        selectedCard = (int) i;
    }

}

void PlayerCards::resetCardSelection() {
    if (selectedCard != -1) {
        cards[selectedCard].setSelected(false);
        selectedCard = -1;
    }
}

void PlayerCards::show() {
    for (auto &card : cards)
        cout << card.getColor() << " " << card.getFigure() << endl;
}

void PlayerCards::draw(Deck *deck) {
    cards.push_back(deck->getCard());
}

void PlayerCards::draw(Card card) {
    cards.push_back(card);
}

Card PlayerCards::discard() {
    Card card = Card();
    if (selectedCard != -1) {
        card = cards[selectedCard];
        cards[selectedCard].setSelected(false);
        cards.erase(cards.begin() + selectedCard);
        selectedCard = -1;
    }
    return card;
}

Card PlayerCards::getSelectedCard() {
    if (selectedCard == -1)
        return Card();
    return cards[selectedCard];
}

void PlayerCards::sortByColor() {
    sort(cards.begin(), cards.end(), [](const Card &card1, const Card &card2) {
        return (card1.getColor() < card2.getColor());
    });
}

void PlayerCards::sortByFigure() {
    sort(cards.begin(), cards.end(), [](const Card &card1, const Card &card2) {
        return (card1.getFigure() < card2.getFigure());
    });
}

