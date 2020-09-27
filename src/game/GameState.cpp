//
// Created by Magdalena on 20.09.2020.
//

#include "GameState.h"

GameState::GameState() {
    cards = PlayerCards();
    cardOnTable = Card();
    numberOfOtherCards = new unsigned int[3]{0, 0, 0};

    bonus = 0;
    turnsToLose = 0;
    request = Card();
    thrown = Card();

    turn = false;
    place = -1;
}

bool GameState::receivedStartPacket() {
    if(cardOnTable.getFigure() != Card::noFigure && cardOnTable.getColor() != Card::noColor)
        return true;
    return false;
}

void GameState::reset() {
    bonus = 0;
    turnsToLose = 0;
    request = Card();
    thrown = Card();

    turn = false;
}

bool GameState::isWaiting() const {
    return waiting > 0;
}

bool GameState::somethingWasThrown() {
    return thrown.getFigure() != Card::noFigure && thrown.getColor() != Card::noColor;
}

Card GameState::discardMove() {
    Card card = cards.discard();
    thrown = card;
    cardOnTable = card;
    return card;
//    if (card.getFigure() != Card::noFigure && card.getColor() != Card::noColor) {
//        if (bonus < 0 && bonus != Server::continueRequest && bonus != Server::Jack)
//            bonus = 0;
//        if (card.getFigure() == 2 || card.getFigure() == 3)
//            bonus += card.getFigure();
//        else if (card.getFigure() == Card::King && card.getColor() == Card::heart) {
//            bonus += 5;
//            moveBack = false;
//        } else if (card.getFigure() == Card::King && card.getColor() == Card::spade) {
//            bonus += 5;
//            moveBack = true;
//        } else if (card.getFigure() == Card::Jack)
//            jackWasThrown = true;
//        else if (card.getFigure() == Card::Ace)
//            aceWasThrown = true;
//        else if (card.getFigure() == Card::Four) {
//            turnsToLose++;
//            fourWasThrown = true;
//            waiting = false;
//        }
//
//        cardOnTable = card;
//
//        if (bonus == Server::Ace)
//            bonus = 0;
//
//        return card;
//    }
}

void GameState::draw(Card card) {
    cards.draw(card);
}

unsigned int GameState::getNumberOfCards() {
    return cards.getNumberOfCards();
}

Card GameState::getCard(int i) {
    return cards.getCard(i);
}

PlayerCards &GameState::getCards(){
    return cards;
}

void GameState::setCards(const PlayerCards &playerCards) {
    cards = playerCards;
}

const Card &GameState::getCardOnTable() const {
    return cardOnTable;
}

void GameState::setCardOnTable(const Card &card) {
    cardOnTable = card;
}

unsigned int *GameState::getNumberOfOtherCards() const {
    return numberOfOtherCards;
}

void GameState::setNumberOfOtherCards(unsigned int *numberOfCards) {
    for(int i = 0; i<3; i++)
        numberOfOtherCards[i] = numberOfCards[i];
}

int GameState::getBonus() const {
    return bonus;
}

void GameState::setBonus(int b) {
    bonus = b;
}

int GameState::getTurnsToLose() const {
    return turnsToLose;
}

void GameState::setTurnsToLose(int turns) {
    turnsToLose = turns;
}

const Card &GameState::getRequest() const {
    return request;
}

void GameState::setRequest(const Card &card) {
    request = card;
}

const Card &GameState::getThrown() const {
    return thrown;
}

void GameState::setThrown(const Card &card) {
    thrown = card;
}

bool GameState::isTurn() const {
    return turn;
}

void GameState::setTurn(bool doMove) {
    turn = doMove;
}

int GameState::getPlace() const {
    return place;
}

void GameState::setPlace(int p) {
    place = p;
}

bool GameState::finished() {
    return place != -1;
}



