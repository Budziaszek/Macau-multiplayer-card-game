//
// Created by Magdalena on 18.09.2020.
//

#ifndef MACAU_SERVER_H
#define MACAU_SERVER_H

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../cards/Card.h"
#include "../cards/Deck.h"
#include "../cards/PlayerCards.h"
#include "../connection/ServerConnection.h"

using namespace std;

class GUI;

class Server {
public:
    Server(unsigned int portToListen = 22, unsigned int places = 4);

    void start();

    bool *playersWhoFinished;
    int numberOfPlayersWhoFinished;

    enum commands {
        finishTurn = 0,
        drawCards,
        discardCard,
        updateStatus,
        missTurn,
        victory
    };
    enum bonusUnderZeroStatus {
        wait = -5,
        continueRequest = -4,
        Four = -3,
        Ace = -2,
        Jack = -1
    };

private:
    Deck deck;
    Card cardOnTable;

    sf::TcpSocket *clients;
    sf::SocketSelector selector;

    unsigned int maxNumberOfPlayers;
    unsigned int numberOfPlayers;
    unsigned int actualPlayer;
    int *playersCardsCounts;

    int win;
    bool moveBack;
    int whoRequested;
    int turnsToLose;
    int figureRequest;
    int colorRequest;

    unsigned int port;
    unsigned int maximumNumberOfPlayers;
    sf::TcpListener listener;
    sf::Packet packet;

    int bonus;

    bool startListening();

    void waitForPlayers();

    void prepareGame();

    void game();

    bool checkContinueGame() const;

    void sendCards(unsigned int clientId, PlayerCards playerCards);

    bool checkSelector(unsigned int clientId);

    void receiveAndResponse(unsigned int clientId);

    void sendCardOnTable(unsigned int clientId);

    void nextPlayer();

    void previousPlayer();

    void sendNumbersOfCards(unsigned int clientId);

    void commandDrawCard(unsigned int clientId);

    void commandDiscard(unsigned int clientId);

    void commandFinishTurn(unsigned int clientId);

    void commandUpdateStatus(unsigned int clientId);

    void commandMissTurn(unsigned int clientId);

    void commandVictory(unsigned int clientId);
};

#endif //MACAU_SERVER_H
