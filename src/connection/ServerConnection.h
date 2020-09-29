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

    enum Command {
        move,
        cards,
        update,
        victory
    };

    static string commandToString(Command command);

    bool connect(const string &ip, unsigned int serverPort = 22);

    void disconnect();

    bool checkSelector();

    void receive(GameState *gameState);

    void discard(GameState *gameState);

    void finishTurn(GameState *gameState, bool forced = false);

    void drawRequest(GameState *gameState);

    void setMove(GameState *gameState);

private:
    unsigned int port;
    sf::IpAddress serverIp;
    sf::TcpSocket server;
    sf::SocketSelector selector;
    sf::Packet packet;

    void actualizeCardOnTable(GameState *gameState);

    void actualizeNumberOfOtherCards(GameState *gameState);

    void actualize(GameState *gameState);

    void actualizeVictory(GameState *gameState);

    void drawCards(GameState *gameState);

    void skipTurn();
};

#endif //MACAU_SERVERCONNECTION_H
