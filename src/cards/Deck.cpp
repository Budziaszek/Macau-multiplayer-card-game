//
// Created by Magdalena on 18.09.2020.
//

#include "Deck.h"
#include "PlayerCards.h"

Deck::Deck() {
    for (int i = 1; i < 5; i++)
        for (int j = 1; j < 14; j++)
            cards.emplace_back(i, j);
}

void Deck::show() {
    cout << "DECK (color, figure)" << endl;
    for (auto &card : cards)
        cout << card.getColor() << " " << card.getFigure() << endl;
}

void Deck::shuffle(unsigned int times) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<unsigned int> intDistrib(0, cards.size());
    for (int i = 0; i < times; i++)
        iter_swap(cards.begin() + intDistrib(gen), cards.begin() + intDistrib(gen));
}

PlayerCards *Deck::deal(unsigned int numberOfPlayers, unsigned int numberOfCardsForEach) {
    auto *playersCards = new PlayerCards[numberOfPlayers];
    for (int i = 0; i < numberOfCardsForEach; i++)
        for (int j = 0; j < numberOfPlayers; j++) {
            playersCards->draw(cards.back());
            cards.pop_back();
        }
    return playersCards;
}

Card Deck::getCard() {
    if(cards.empty()) {
        cards.swap(usedCards);
        shuffle();
    }
    Card card = cards.back();
    cards.pop_back();
    return card;
}

void Deck::addCard(Card card) {
    usedCards.push_back(card);
}
