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

    Card discardMove();

    void draw(Card card);

    unsigned int getNumberOfCards();

    Card getCard(int i);

    PlayerCards &getCards();

    void setCards(const PlayerCards &playerCards);

    const Card &getCardOnTable() const;

    void setCardOnTable(const Card &card);

    unsigned int *getNumberOfOtherCards() const;

    void setNumberOfOtherCards(unsigned int *numberOfCards);

    int getBonus() const;

    void setBonus(int b);

    int getTurnsToLose() const;

    void setTurnsToLose(int turns);

    const Card &getRequest() const;

    void setRequest(const Card &card);

    const Card &getThrown() const;

    void setThrown(const Card &card);

    bool isTurn() const;

    void setTurn(bool doMove);

    int getPlace() const;

    void setPlace(int p);

    bool finished();

private:
    PlayerCards cards;
    Card cardOnTable;
    unsigned int *numberOfOtherCards;

    int bonus;
    int turnsToLose;
    Card request;
    Card thrown;
    int waiting;

    bool turn;
    int place;
};

#endif //MACAU_GAMESTATE_H
