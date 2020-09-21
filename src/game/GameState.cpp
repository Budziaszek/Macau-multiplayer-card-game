//
// Created by Magdalena on 20.09.2020.
//

#include "GameStatus.h"

GameStatus::GameStatus() {
    myPlace = -1;
    numberOfOtherCards = new unsigned int[3];
    moveBack = false;
    itsMyTurn = false;
    somethingWasThrown = false;
    jackWasThrown = false;
    aceWasThrown = false;
    fourWasThrown = false;
    waiting = false;
    figureOfAlreadyThrown = Card::noFigure;
}

int GameStatus::getMyPlace() const {
    return myPlace;
}

void GameStatus::setMyPlace(int myPlace) {
    GameStatus::myPlace = myPlace;
}

bool GameStatus::isMoveBack() const {
    return moveBack;
}

void GameStatus::setMoveBack(bool moveBack) {
    GameStatus::moveBack = moveBack;
}

int GameStatus::getFigureRequest() const {
    return figureRequest;
}

void GameStatus::setFigureRequest(int figureRequest) {
    GameStatus::figureRequest = figureRequest;
}

int GameStatus::getColorRequest() const {
    return colorRequest;
}

void GameStatus::setColorRequest(int colorRequest) {
    GameStatus::colorRequest = colorRequest;
}

int GameStatus::getTurnsToLose() const {
    return turnsToLose;
}

void GameStatus::setTurnsToLose(int turnsToLose) {
    GameStatus::turnsToLose = turnsToLose;
}

bool GameStatus::isItsMyTurn() const {
    return itsMyTurn;
}

void GameStatus::setItsMyTurn(bool itsMyTurn) {
    GameStatus::itsMyTurn = itsMyTurn;
}

bool GameStatus::isWaiting() const {
    return waiting;
}

void GameStatus::setWaiting(bool waiting) {
    GameStatus::waiting = waiting;
}

bool GameStatus::isWaitPermanently() const {
    return waitPermanently;
}

void GameStatus::setWaitPermanently(bool waitPermanently) {
    GameStatus::waitPermanently = waitPermanently;
}

unsigned int GameStatus::getFigureOfAlreadyThrown() const {
    return figureOfAlreadyThrown;
}

void GameStatus::setFigureOfAlreadyThrown(unsigned int figureOfAlreadyThrown) {
    GameStatus::figureOfAlreadyThrown = figureOfAlreadyThrown;
}

bool GameStatus::isSomethingWasThrown() const {
    return somethingWasThrown;
}

void GameStatus::setSomethingWasThrown(bool somethingWasThrown) {
    GameStatus::somethingWasThrown = somethingWasThrown;
}

bool GameStatus::isSomethingWasDone() const {
    return somethingWasDone;
}

void GameStatus::setSomethingWasDone(bool somethingWasDone) {
    GameStatus::somethingWasDone = somethingWasDone;
}

bool GameStatus::isJackWasThrown() const {
    return jackWasThrown;
}

void GameStatus::setJackWasThrown(bool jackWasThrown) {
    GameStatus::jackWasThrown = jackWasThrown;
}

bool GameStatus::isAceWasThrown() const {
    return aceWasThrown;
}

void GameStatus::setAceWasThrown(bool aceWasThrown) {
    GameStatus::aceWasThrown = aceWasThrown;
}

bool GameStatus::isFourWasThrown() const {
    return fourWasThrown;
}

void GameStatus::setFourWasThrown(bool fourWasThrown) {
    GameStatus::fourWasThrown = fourWasThrown;
}

int GameStatus::getBonus() const {
    return bonus;
}

void GameStatus::setBonus(int bonus) {
    GameStatus::bonus = bonus;
}

unsigned int *GameStatus::getNumberOfOtherCards() const {
    return numberOfOtherCards;
}

void GameStatus::setNumberOfOtherCards(unsigned int *numberOfOtherCards) {
    GameStatus::numberOfOtherCards = numberOfOtherCards;
}

const Card &GameStatus::getCardOnTable() const {
    return cardOnTable;
}

void GameStatus::setCardOnTable(const Card &cardOnTable) {
    GameStatus::cardOnTable = cardOnTable;
}

void GameStatus::reset() {
    bonus = 0;
    turnsToLose = 0;
    moveBack = false;
    jackWasThrown = false;
    aceWasThrown = false;
    fourWasThrown = false;
    itsMyTurn = false;
    somethingWasThrown = false;
    figureOfAlreadyThrown = Card::noFigure;
    somethingWasDone = false;
}

Card GameStatus::discardMove() {
    somethingWasThrown = true;
    Card card = cards.discard();

    if (card.getFigure() != Card::noFigure && card.getColor() != Card::noColor) {
        if (bonus < 0 && bonus != Server::continueRequest && bonus != Server::Jack)
            bonus = 0;
        if (card.getFigure() == 2 || card.getFigure() == 3)
            bonus += card.getFigure();
        else if (card.getFigure() == Card::King && card.getColor() == Card::heart) {
            bonus += 5;
            moveBack = false;
        } else if (card.getFigure() == Card::King && card.getColor() == Card::spade) {
            bonus += 5;
            moveBack = true;
        } else if (card.getFigure() == Card::Jack)
            jackWasThrown = true;
        else if (card.getFigure() == Card::Ace)
            aceWasThrown = true;
        else if (card.getFigure() == Card::Four) {
            turnsToLose++;
            fourWasThrown = true;
            waiting = false;
        }

        cardOnTable = card;
        figureOfAlreadyThrown = card.getFigure();
        somethingWasDone = true;

        if (bonus == Server::Ace)
            bonus = 0;

        return card;
    }
}

void GameStatus::draw(Card card) {
    cards.draw(card);
}

int GameStatus::getNumberOfCards() const {
    return cards.numberOfCards;
}

Card GameStatus::getCard(int i) {
    return cards.checkOne(i);
}

void GameStatus::resetChoosenCard() {
    cards.resetChoosen();
}
