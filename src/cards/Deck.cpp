//
// Created by Magdalena on 18.09.2020.
//

#include <chrono>
#include "Deck.h"
#include "PlayerCards.h"

Deck::Deck() {
    for (int i = 1; i < 5; i++)
        for (int j = 1; j < 14; j++)
            cards.emplace_back(i, j);
    seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(cards.begin(),cards.end(), std::default_random_engine(seed));
}

void Deck::show() {
    cout << "DECK (color, figure)" << endl;
    for (auto &card : cards)
        cout << card.getColor() << " " << card.getFigure() << endl;
}

PlayerCards *Deck::deal(unsigned int numberOfPlayers, unsigned int numberOfCardsForEach) {
    auto* playersCards = new PlayerCards[numberOfPlayers];
    for (int i = 0; i < numberOfCardsForEach; i++)
        for (int j = 0; j < numberOfPlayers; j++) {
            playersCards[j].draw(cards.back());
            cards.pop_back();
        }
    return playersCards;
}

Card Deck::getCard() {
    if (cards.empty()) {
        cout << "EMPTY" << endl;
        cards.swap(usedCards);
        shuffle(cards.begin(),cards.end(), std::default_random_engine(seed));
    }
    auto card = cards.back();
    cards.pop_back();
    return card;
}

void Deck::addCard(Card card) {
    usedCards.push_back(card);
}
