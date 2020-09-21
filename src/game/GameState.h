//
// Created by Magdalena on 20.09.2020.
//

#ifndef MACAU_GAMESTATUS_H
#define MACAU_GAMESTATUS_H

#include "../cards/Card.h"
#include "../connection/Server.h"

class GameStatus {
public:
    GameStatus();

    void reset();

    Card discardMove();

    void draw(Card card);

    int getMyPlace() const;

    void setMyPlace(int myPlace);

    bool isMoveBack() const;

    void setMoveBack(bool moveBack);

    int getNumberOfCards() const;

    Card getCard(int i);

    void resetChoosenCard();

    int getFigureRequest() const;

    void setFigureRequest(int figureRequest);

    int getColorRequest() const;

    void setColorRequest(int colorRequest);

    int getTurnsToLose() const;

    void setTurnsToLose(int turnsToLose);

    bool isItsMyTurn() const;

    void setItsMyTurn(bool itsMyTurn);

    bool isWaiting() const;

    void setWaiting(bool waiting);

    bool isWaitPermanently() const;

    void setWaitPermanently(bool waitPermanently);

    unsigned int getFigureOfAlreadyThrown() const;

    void setFigureOfAlreadyThrown(unsigned int figureOfAlreadyThrown);

    bool isSomethingWasThrown() const;

    void setSomethingWasThrown(bool somethingWasThrown);

    bool isSomethingWasDone() const;

    void setSomethingWasDone(bool somethingWasDone);

    bool isJackWasThrown() const;

    void setJackWasThrown(bool jackWasThrown);

    bool isAceWasThrown() const;

    void setAceWasThrown(bool aceWasThrown);

    bool isFourWasThrown() const;

    void setFourWasThrown(bool fourWasThrown);

    int getBonus() const;

    void setBonus(int bonus);

    unsigned int *getNumberOfOtherCards() const;

    void setNumberOfOtherCards(unsigned int *numberOfOtherCards);

    const Card &getCardOnTable() const;

    void setCardOnTable(const Card &cardOnTable);

private:
    int myPlace;
    bool moveBack;
    int figureRequest;
    int colorRequest;
    int turnsToLose;

    bool itsMyTurn;
    bool waiting;
    bool waitPermanently;
    unsigned int figureOfAlreadyThrown;
    bool somethingWasThrown;
    bool somethingWasDone;
    bool jackWasThrown;
    bool aceWasThrown;
    bool fourWasThrown;
    int bonus;

    unsigned int *numberOfOtherCards;
    Card cardOnTable;
    PlayerCards cards;
};

#endif //MACAU_GAMESTATUS_H
