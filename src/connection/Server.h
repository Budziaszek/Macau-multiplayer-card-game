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

    enum Command {
        draw,
        discard,
        finish,
        skip
    };

    static string commandToString(Command command);

private:
    Deck deck;
    Card cardOnTable;

    sf::TcpSocket *clients;
    sf::SocketSelector selector;

    unsigned int maxNumberOfPlayers;
    unsigned int numberOfPlayers;
    unsigned int actualPlayer;
    unsigned int playersWhoFinished;
    int *playersCardsCounts;

    int win;
    bool moveBack;
    int whoRequested;
    int turnsToLose;
    Card request;

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

    bool checkSelector(unsigned int clientId);

    void receiveAndResponse(unsigned int clientId);

    void nextPlayer();

    void previousPlayer();

    void commandDraw(unsigned int clientId, int b = 0);

    void commandDiscard(unsigned int clientId);

    void commandFinishTurn(unsigned int clientId);

    void sendTurnInformation();

    void sendUpdateToAll();

    void sendUpdate(unsigned int clientId);

    void commandMissTurn();

    void sendVictoryInformation(unsigned int clientId);
};

#endif //MACAU_SERVER_H
