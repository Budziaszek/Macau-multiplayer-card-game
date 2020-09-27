//
// Created by Magdalena on 18.09.2020.
//

#include "Server.h"

Server::Server(unsigned int portToListen, unsigned int places) {
    maxNumberOfPlayers = places;
    deck = Deck();
    cardOnTable = Card();

    clients = new sf::TcpSocket[maxNumberOfPlayers];

    numberOfPlayers = 0;
    actualPlayer = -1;
    playersCardsCounts = new int[maxNumberOfPlayers];
    for(int i = 0; i< maxNumberOfPlayers; i++)
        playersCardsCounts[i] = 0;

    whoRequested = -1;
    port = portToListen;

    maximumNumberOfPlayers = maxNumberOfPlayers;
    bonus = 0;

    win = 1;
    playersWhoFinished = new bool[numberOfPlayers];
    for (int i = 0; i < numberOfPlayers; i++)
        playersWhoFinished[i] = false;
    numberOfPlayersWhoFinished = 0;
}

void Server::start(){
    startListening();
    waitForPlayers();
    prepareGame();
    game();
}

bool Server::startListening() {
    cout << "SERVER" << endl;
    if (listener.listen(port) != sf::Socket::Done) {
        cout << "Can not start listening on port: " << port << endl;
        system("PAUSE");
        return false;
    } else {
        cout << "Listening on port: " << port << endl;
        cout << "Pubic server ip: " << sf::IpAddress::getPublicAddress() << endl;
        cout << "Local server ip: " << sf::IpAddress::getLocalAddress() << endl;
        return true;
    }
}

void Server::waitForPlayers() {
    do {
        cout << "Enter number of players (2-4): ";
        cin >> numberOfPlayers;
    } while (numberOfPlayers > 4 || numberOfPlayers < 2);
    cout << "Waiting for players (" << numberOfPlayers << ")" << endl;
    for (int i = 0; i < numberOfPlayers; i++) {
        listener.accept(clients[i]);
        selector.add(clients[i]);
        std::cout << "Player joined. Ip: " << clients[i].getRemoteAddress() << std::endl;
    }
}

void Server::prepareGame(){
    cout << "Prepare game" << endl;
    deck.shuffle();
    PlayerCards *playerCards = deck.deal(numberOfPlayers, 5);
    for(int i = 0; i< numberOfPlayers; i++)
        playersCardsCounts[i] = 5;
    do {
        cardOnTable = deck.getCard();
    } while (cardOnTable.isSpecial());


    for (int i = 0; i < numberOfPlayers; i++) {
        sendCards(i, playerCards[i]);
        sendCardOnTable(i);
        sendNumbersOfCards(i);
    }

    packet.clear();
    packet << ServerConnection::move;
    // TODO: random first player
    actualPlayer = 0;
    if (clients[actualPlayer].send(packet) == sf::Socket::Done)
        cout << "Start." << endl;
}

bool Server::checkContinueGame() const{
    //TODO check if players are connected
    if (numberOfPlayersWhoFinished == numberOfPlayers - 1)
        return false;
    else
        return true;
}

void Server::game() {
    while (checkContinueGame()){
        if(checkSelector(actualPlayer))
            receiveAndResponse(actualPlayer);
    }
    listener.close();
    cout << "Game finished." << endl;
    system("PAUSE");
}

void Server::sendCards(unsigned int clientId, PlayerCards playerCards) {
    packet.clear();
    packet << ServerConnection::cards << playerCards.getNumberOfCards();
    for (int i = 0; i < playerCards.getNumberOfCards(); i++)
        packet << playerCards.getCard(i).getColor() << playerCards.getCard(i).getFigure();
    clients[clientId].send(packet);
}

void Server::sendCardOnTable(unsigned int clientId) {
    packet.clear();
    packet << ServerConnection::cardOnTable;
    packet << cardOnTable.getColor() << cardOnTable.getFigure() << bonus;
    clients[clientId].send(packet);
}

void Server::sendNumbersOfCards(unsigned int clientId) {
    packet.clear();
    packet << ServerConnection::opponents;
    for (int i = 1; i < maxNumberOfPlayers; i++)
        packet << playersCardsCounts[(clientId + i) % maxNumberOfPlayers];
    clients[clientId].send(packet);
}

bool Server::checkSelector(unsigned int clientId) {
    if (selector.wait(sf::microseconds(100)))
        return selector.isReady(clients[clientId]);
}

void Server::receiveAndResponse(unsigned int clientId) {
    packet.clear();
    if (clients[clientId].receive(packet) == sf::Socket::Done) {
        int commandId;
        packet >> commandId;
        switch (commandId) {
            case Server::drawCards:
                commandDrawCard(clientId);
                break;
            case Server::discardCard:
                commandDiscard(clientId);
                break;
            case Server::finishTurn:
                commandFinishTurn(clientId);
                break;
            case Server::updateStatus:
                commandUpdateStatus(clientId);
                break;
            case Server::missTurn:
                commandMissTurn(clientId);
                break;
            case Server::victory:
                commandVictory(clientId);
                break;
            default:
                return;
        }
    }
}

void Server::nextPlayer() {
    if (actualPlayer == numberOfPlayers - 1)
        actualPlayer = 0;
    else
        actualPlayer++;
    if (playersWhoFinished[actualPlayer] == true)
        nextPlayer();
}

void Server::previousPlayer() {
    if (actualPlayer == 0)
        actualPlayer = numberOfPlayers - 1;
    else
        actualPlayer--;
    if (playersWhoFinished[actualPlayer] == true)
        previousPlayer();
}

void Server::commandDrawCard(unsigned int clientId) {
    packet.clear();
    packet << ServerConnection::cards;
    packet << 1;

    Card card = deck.getCard();
    packet << card.getColor() << card.getFigure();
    playersCardsCounts[clientId] += 1;

    while (clients[clientId].send(packet) != sf::Socket::Done);

    for (int i = 0; i < numberOfPlayers; i++) {
        sendCardOnTable(i);
        sendNumbersOfCards(i);
    }

}

void Server::commandDiscard(unsigned int clientId) {
    int color, figure;
    packet >> color >> figure;
    cardOnTable = Card(color, figure);
    playersCardsCounts[clientId] -= 1;

    for (int i = 0; i < numberOfPlayers; i++) {
        sendCardOnTable(i);
        sendNumbersOfCards(i);
    }
}

void Server::commandFinishTurn(unsigned int clientId) {
    packet >> bonus;
    if (bonus == Server::Jack) {
        packet >> figureRequest;
        whoRequested = clientId;
    } else if (bonus == Server::Ace) {
        packet >> colorRequest;
    } else if (bonus == Server::Four) {
        packet >> turnsToLose;
    } else if (bonus == Server::continueRequest) {
        if (whoRequested != actualPlayer)
            bonus = Server::Jack;
        if (whoRequested == actualPlayer) {
            bonus = 0;
            whoRequested = -1;
        }
    }
    moveBack = false;
    packet >> moveBack;
    if (moveBack)
        previousPlayer();
    else
        nextPlayer();

    packet.clear();
    packet << ServerConnection::move;
    while (clients[actualPlayer].send(packet) != sf::Socket::Done);
    packet.clear();
    packet << -1;
    while (clients[clientId].send(packet) != sf::Socket::Done);
}

void Server::commandUpdateStatus(unsigned int clientId) {
    packet.clear();
    packet << ServerConnection::bonus << bonus;
    if (bonus == Server::Jack)
        packet << figureRequest;
    else if (bonus == Server::Four)
        packet << turnsToLose;
    else if (bonus == Server::Ace)
        packet << colorRequest;
    packet << moveBack;
    while (clients[actualPlayer].send(packet) != sf::Socket::Done);
}

void Server::commandMissTurn(unsigned int clientId) {
    if (moveBack)
        previousPlayer();
    else
        nextPlayer();

    packet.clear();
    packet << ServerConnection::move;
    while (clients[actualPlayer].send(packet) != sf::Socket::Done);
    packet.clear();
    packet << -1;
    while (clients[clientId].send(packet) != sf::Socket::Done);
}

void Server::commandVictory(unsigned int clientId) {
    packet.clear();
    packet << ServerConnection::victory << win;
    win++;
    while (clients[actualPlayer].send(packet) != sf::Socket::Done);
    playersWhoFinished[actualPlayer] = true;
    numberOfPlayersWhoFinished++;

    if (numberOfPlayersWhoFinished == numberOfPlayers - 1) {
        packet.clear();
        packet << ServerConnection::victory << win;
        win++;
        for (int i = 0; i < numberOfPlayers; i++)
            if (playersWhoFinished[i] != true) {
                while (clients[i].send(packet) != sf::Socket::Done);
                break;
            }
    }

}
