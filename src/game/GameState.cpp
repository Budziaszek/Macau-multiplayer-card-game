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

const Card &GameState::getRequest() const {
    return request;
}

void GameState::setRequest(const Card &card) {
    request = card;
}

Card GameState::getThrown(){
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

bool GameState::finished() const {
    return place != -1;
}

Card GameState::getSelectedCard() {
    return cards.getSelectedCard();
}

Card GameState::discard() {
    thrown = getSelectedCard();
    return cards.discard();
}

bool GameState::canDraw() {
    if (turnsToLose > 0)
        return false;
    if (thrown.isFullyDefined())
        return false;
    if (request.getFigure() == Card::four)
        return false;
    return true;
}

bool GameState::canFinish() {
    return thrown.isFullyDefined() || turnsToLose > 0;
}

bool GameState::canDiscard(Card card){
    if (card.isFullyDefined()) {
        if (thrown.isFullyDefined()) {
            if (thrown.getFigure() == card.getFigure())
                return true;
            return false;
        }
        if (isThereRequest()){
            if(cardMatchesRequest(card))
                return true;
            else if(isThereFigureRequest() && card.getFigure() == Card::jack)
                return true;
            else if(isThereColorRequest() && card.getFigure() == Card::ace)
                return true;
            return false;
        }
        if (isBraveCardRequired()) {
            if (card.isBrave() && cardMatchesCardOnTable(card))
                return true;
            return false;
        }
        if (isFourRequired()) {
            if (card.getFigure() == Card::four)
                return true;
            return false;
        }
        if (cardMatchesCardOnTable(card)) {
            return true;
        }
        return false;
    }
    return false;
}

bool GameState::isThereRequest() {
    return isThereColorRequest() || isThereFigureRequest();
}

bool GameState::isThereFigureRequest() {
    return request.getFigure() != Card::noFigure;
}

bool GameState::isThereColorRequest() {
    return request.getColor() != Card::noColor;
}

bool GameState::cardMatchesRequest(Card card) {
    if(!isThereRequest())
        return true;
    if(request.getFigure() == card.getFigure())
        return true;
    if(request.getColor() == card.getColor())
        return true;
    return false;
}

bool GameState::cardMatchesCardOnTable(Card card) {
    if(cardOnTable.getFigure() == card.getFigure())
        return true;
    if(cardOnTable.getColor() == card.getColor())
        return true;
    return false;
}

bool GameState::isBraveCardRequired() const {
    return bonus > 0;
}

bool GameState::isFourRequired() {
    return turnsToLose > 0;
}

int GameState::getTurnsToLose() const {
    return turnsToLose;
}

void GameState::setTurnsToLose(int turns) {
    GameState::turnsToLose = turns;
}

int GameState::getWaiting() const {
    if(finished())
        return 1;
    return waiting;
}

void GameState::setWaiting(int w) {
    GameState::waiting = w;
}



