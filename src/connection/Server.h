//
// Created by Magdalena on 18.09.2020.
//

#ifndef MACAU_SERVER_H
#define MACAU_SERVER_H

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../cards/Card.h"
#include "../cards/PlayerCards.h"

using namespace std;

class Server
{
public:
    Server(unsigned int portToListen = 22, unsigned int maxNumberOfPlayers = 4);
    bool startListening();
    void waitForPlayers(unsigned int definedNumberOfPlayers);
    void game();
    void sendCards(unsigned int clientId);
    bool recieveSomethingAndResponse(unsigned int clientId);
    void sendCardOnTable(unsigned int clientId);
    void nextPlayer();
    void previousPlayer();
    void sendNumbersOfCards(unsigned int clientId);
    bool* playersWhoFinished;
    int numberOfPlayersWhoFinished;

    enum commands
    {
        finish = 0, //zakonczenie ruchu, server ma przejsc do nastepnego gracza
        draw, //server ma da� graczowi okreslona ilosc kart (1 jesli brak "bonus�w")
        discard, //+ argumenty opisujace wyrzucona karte
        giveBonus, //odsyla informacje o bonusie
        loseTurn //gracz zada utraty kolejki (ma do odstania z powodu 4)
    };
    enum bonusUnderZeroStatus
    {
        winStatus = -6,
        wait = -5,
        continueReqest = -4,
        Four = -3,
        Ace = -2,
        Jack = -1
    };

private:
    int win;
    bool moveBack;
    int whoRequested;
    int turnsToLose;
    int figureRequest;
    int colorRequest;
    unsigned int actualPlayer;
    Card cardOnTable;
    sf::TcpSocket* client;
    unsigned int port;
    unsigned int numberOfPlayers;
    unsigned int maximumNumberOfPlayers;
    PlayerCards* playersCards;
    sf::TcpListener listener;
    sf::Packet packet;

    int bonus;
    Deck deck;
};

#endif //MACAU_SERVER_H
