//
// Created by Magdalena on 19.09.2020.
//

#include "ServerConnection.h"

ServerConnection::ServerConnection() {

}

bool ServerConnection::connect(const string &ip, unsigned int serverPort) {
    port = serverPort;
    serverIp = sf::IpAddress(ip);
    if (server.connect(serverIp, port) == sf::Socket::Done) {
        selector.add(server);
        return true;
    }
    return false;
}

void ServerConnection::disconnect() {
    server.disconnect();
}

void ServerConnection::checkTurn(GameState *gameStatus) {
    if (checkSelector())
        receive(gameStatus);
//            if (gameStatus->isItsMyTurn() && !gameStatus->isWaitPermanently()) {
//                packet.clear();
//                packet << Server::updateStatus;
//                server.send(packet);
//                receiveSomething(gameStatus);
//            }
}

void ServerConnection::actualizeCardOnTable(GameState *gameState) {
    int color, figure;
    packet >> color >> figure;
    gameState->setCardOnTable(Card(color, figure));
}

void ServerConnection::actualizeNumberOfOtherCards(GameState *gameState) {
    unsigned int n1, n2, n3;
    packet >> n1 >> n2 >> n3;
    unsigned int numbers[] = {n1, n2, n3};
    gameState->setNumberOfOtherCards(numbers);
}

void ServerConnection::actualizeBonus(GameState *gameState) {
    int bonus, turnsToLose = 0;
    unsigned int figureRequest = Card::noFigure;
    unsigned int colorRequest = Card::noColor;
    Card request = Card();
    packet >> bonus;
    gameState->setBonus(bonus);
    if (bonus == Server::Jack)
        packet >> figureRequest;
    else if (bonus == Server::Ace)
        packet >> colorRequest;
    else if (bonus == Server::Four)
        packet >> turnsToLose;
    gameState->setRequest(Card(figureRequest, colorRequest));
    gameState->setTurnsToLose(turnsToLose);
}

void ServerConnection::actualizeVictory(GameState *gameState) {
    int place;
    packet >> place;
    gameState->setPlace(place);
}

bool ServerConnection::checkSelector() {
    if (selector.wait(sf::microseconds(100)))
        return selector.isReady(server);
}

void ServerConnection::receive(GameState *gameState) {
    packet.clear();
    if(server.receive(packet) == sf::Socket::Done) {
        int commandId;
        packet >> commandId;
        cout << commandId << endl;
        switch (commandId) {
            case (ServerConnection::move):
                gameState->setTurn(true);
                break;
            case (ServerConnection::cardOnTable):
                actualizeCardOnTable(gameState);
                break;
            case (ServerConnection::cards):
                drawCards(gameState);
                break;
            case (ServerConnection::opponents):
                actualizeNumberOfOtherCards(gameState);
                break;
            case (ServerConnection::bonus):
                actualizeBonus(gameState);
                break;
            case (ServerConnection::victory):
                actualizeVictory(gameState);
                break;
            default:
                return;
        }
    }
}

void ServerConnection::drawCards(GameState *gameState) {
    int numberOfCardsToTake;
    packet >> numberOfCardsToTake;
    for (int i = 0; i < numberOfCardsToTake; i++) {
        int color, figure;
        packet >> color >> figure;
        Card card = Card(color, figure);
        gameState->draw(card);
    }
}

void ServerConnection::discard(Card card) {
    packet.clear();
    packet << Server::discardCard << card.getColor() << card.getFigure();
    while (server.send(packet) != sf::Socket::Done);
}


void ServerConnection::finishTurn(GameState *gameStatus) {
    packet.clear();
    packet << Server::finishTurn;

//    if (!gameStatus->isJackWasThrown() && gameStatus->getBonus() == Server::Jack)
//        packet << Server::continueRequest;
//    else if (gameStatus->isFourWasThrown())
//        packet << Server::Four << gameStatus->getTurnsToLose();
//    else
//        packet << gameStatus->getBonus();
//    if (gameStatus->isJackWasThrown())
//        packet << gameStatus->getFigureRequest();
//    if (gameStatus->isAceWasThrown() || gameStatus->getBonus() == Server::Ace)
//        packet << gameStatus->getColorRequest();
//    packet << gameStatus->isMoveBack();

    while (server.send(packet) != sf::Socket::Done);

}

void ServerConnection::loseTurn() {
    packet.clear();
    packet << Server::missTurn;
    while (server.send(packet) != sf::Socket::Done);
}

void ServerConnection::finish() {
    packet.clear();
    packet << Server::finishTurn << 0 << 0;
    while (server.send(packet) != sf::Socket::Done);
}



