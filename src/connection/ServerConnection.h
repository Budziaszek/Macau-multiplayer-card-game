//
// Created by Magdalena on 19.09.2020.
//

#ifndef MACAU_SERVERCONNECTION_H
#define MACAU_SERVERCONNECTION_H

#include "SFML/Network.hpp"
#include "../cards/Card.h"
#include "../game/GameState.h"

using namespace std;

class GameState;

class ServerConnection {
public:
    ServerConnection();

    enum commands {
        move = 0,
        cards,
        cardOnTable,
        opponents,
        bonus,
        victory
    };

    bool connect(const string &ip, unsigned int serverPort = 22);

    void disconnect();

    bool checkSelector();

    void receive(GameState *gameState);

private:
    unsigned int port;
    sf::IpAddress serverIp;
    sf::TcpSocket server;
    sf::SocketSelector selector;
    sf::Packet packet;

    void checkTurn(GameState *gameState);

    void actualizeCardOnTable(GameState *gameState);

    void drawCards(GameState *gameState);

    void actualizeNumberOfOtherCards(GameState *gameState);

    void actualizeBonus(GameState *gameState);

    void actualizeVictory(GameState *gameState);

    void discard(Card card);

    void finishTurn(GameState *gameState);

    void loseTurn();

    void finish();
};

#endif //MACAU_SERVERCONNECTION_H
