//
// Created by Magdalena on 19.09.2020.
//

#include "ServerConnection.h"

ServerConnection::ServerConnection() {

}

string ServerConnection::commandToString(Command command){
    switch(command){
        case move: return "move";
        case cards: return "cards";
        case update: return "update";
        case victory: return "victory";
    }
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

void ServerConnection::actualize(GameState *gameState) {
    int bonus, turnsToLose;
    unsigned int colorRequest, figureRequest;

    actualizeCardOnTable(gameState);

    packet >> bonus;
    packet >> turnsToLose;

    packet >> colorRequest;
    packet >> figureRequest;

    actualizeNumberOfOtherCards(gameState);

    gameState->setRequest(Card(colorRequest, figureRequest));
    gameState->setBonus(bonus);
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
        cout << commandToString(Command(commandId)) << endl;
        switch (commandId) {
            case (ServerConnection::move):
                setMove(gameState);
                break;
            case (ServerConnection::cards):
                drawCards(gameState);
                break;
            case (ServerConnection::update):
                actualize(gameState);
                break;
            case (ServerConnection::victory):
                actualizeVictory(gameState);
                break;
            default:
                return;
        }
    }
}

void ServerConnection::setMove(GameState *gameState){
    if(gameState->isWaiting())
    {
        gameState->setWaiting(gameState->getWaiting() - 1);
        skipTurn();
    }
    else
        gameState->setTurn(true);
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

void ServerConnection::discard(GameState *gameState) {
    Card card = gameState->discard();
    packet.clear();
    packet << Server::discard;
    packet << card.getColor() << card.getFigure();
    server.send(packet);
}

void ServerConnection::finishTurn(GameState *gameState, bool forced) {
    if (forced || gameState->canFinish()) {
        if(!gameState->getThrown().isFullyDefined()){
            gameState->setWaiting(gameState->getTurnsToLose());
            gameState->setTurnsToLose(0);
        }
        packet.clear();
        packet << Server::finish;
        packet << gameState->getTurnsToLose();
        packet << gameState->getRequest().getColor() << gameState->getRequest().getFigure();
        server.send(packet);
        gameState->reset();
        gameState->getCards().resetCardSelection();
    }
}

void ServerConnection::skipTurn() {
    packet.clear();
    packet << Server::skip;
    server.send(packet);
}

void ServerConnection::drawRequest(GameState *gameState) {
    if (gameState->canDraw()) {
        packet.clear();
        packet << Server::draw;
        server.send(packet);
        finishTurn(gameState, true);
    }
}





