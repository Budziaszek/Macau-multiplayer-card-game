//
// Created by Magdalena on 20.09.2020.
//

#ifndef MACAU_GAMESTATE_H
#define MACAU_GAMESTATE_H

#include "../cards/Card.h"
#include "../connection/Server.h"

class GameState {
public:
    GameState();

    bool receivedStartPacket();

    void reset();

    bool isWaiting() const;

    bool somethingWasThrown();

    void draw(Card card);

    unsigned int getNumberOfCards();

    Card getCard(int i);

    Card getSelectedCard();

    Card discard();

    PlayerCards &getCards();

    void setCards(const PlayerCards &playerCards);

    const Card &getCardOnTable() const;

    void setCardOnTable(const Card &card);

    unsigned int *getNumberOfOtherCards() const;

    void setNumberOfOtherCards(unsigned int *numberOfCards);

    int getBonus() const;

    void setBonus(int b);

    const Card &getRequest() const;

    void setRequest(const Card &card);

    Card getThrown();

    void setThrown(const Card &card);

    bool isTurn() const;

    void setTurn(bool doMove);

    int getPlace() const;

    void setPlace(int p);

    bool finished() const;

    bool canDraw();

    bool isThereFigureRequest();

    bool isThereColorRequest();

    bool isThereRequest();

    bool cardMatchesRequest(Card card);

    bool cardMatchesCardOnTable(Card card);

    bool canFinish();

    bool canDiscard(Card card);

    bool isBraveCardRequired() const;

    bool isFourRequired();

    int getTurnsToLose() const;

    void setTurnsToLose(int turns);

    int getWaiting() const;

    void setWaiting(int w);

private:
    PlayerCards cards;
    Card cardOnTable;
    unsigned int *numberOfOtherCards;

    int bonus;
    Card request;
    Card thrown;
    int turnsToLose;

private:
    int waiting;

    bool turn;
    int place;
};

#endif //MACAU_GAMESTATE_H
