//
// Created by Magdalena on 19.09.2020.
//

#ifndef MACAU_SERVERCONNECTION_H
#define MACAU_SERVERCONNECTION_H

#include "SFML/Network.hpp"
#include "../cards/Card.h"
#include "../game/GameStatus.h"

using namespace std;

class GameStatus;

class ServerConnection {
public:
    enum commands {
        move = 0, //informacja o tym, ze kolej na gracza
        cardsList, //arg: ilosc kart (przy rozdaniu, ciagnieciu), kolor, figura, kolor....
        cardOnTableActualization, //kolor, figura
        numbersOfCards, //3 liczby, informuja o ilosci kart innych
        catchBonus, //przyjmuje bonus
        place //przyjmuje informacje o zaj�tym miejscu (po zwyci�stwie)
    };

    explicit ServerConnection(unsigned int serverPort = 22);
    bool connect(const string& ip);
    bool receiveStartPacket(GameStatus *gameStatus);
    void disconnect();
    void checkTurn(GameStatus *gameStatus);
    bool receiveSomething(GameStatus *gameStatus);
    void drawCards(GameStatus *gameStatus);
    void discard(Card card);
    void victory(GameStatus *gameStatus);
    void finishTurn(GameStatus *gameStatus);
    void loseTurn();
    void finish();
private:
    unsigned int port;
    sf::IpAddress serverIp;
    sf::TcpSocket server;
    sf::SocketSelector selector;
    sf::Packet packet;

};

#endif //MACAU_SERVERCONNECTION_H
