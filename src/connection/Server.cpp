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
    for (int i = 0; i < maxNumberOfPlayers; i++)
        playersCardsCounts[i] = 0;

    whoRequested = -1;
    port = portToListen;

    maximumNumberOfPlayers = maxNumberOfPlayers;
    bonus = 0;
    moveBack = false;
    turnsToLose = 0;

    playersWhoFinished = 0;

    win = 1;
}

string Server::commandToString(Command command) {
    switch (command) {
        case finish:
            return "finish";
        case draw:
            return "draw";
        case discard:
            return "discard";
        case skip:
            return "skip";
    }
}

void Server::start() {
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

void Server::prepareGame() {
    cout << "Prepare game." << endl;

    do {
        cardOnTable = deck.getCard();
    } while (cardOnTable.isSpecial());

    for (int i = 0; i < numberOfPlayers; i++)
        commandDraw(i, 5);
    sendUpdateToAll();

    packet.clear();
    packet << ServerConnection::move;
    // TODO: random first player
    actualPlayer = 0;
    if (clients[actualPlayer].send(packet) == sf::Socket::Done)
        cout << "Start." << endl;
}

bool Server::checkContinueGame() const {
    //TODO check if players are connected
    return playersWhoFinished != numberOfPlayers;
}

void Server::game() {
    while (checkContinueGame()) {
        if (checkSelector(actualPlayer))
            receiveAndResponse(actualPlayer);
    }
    listener.close();
    cout << "Game finished." << endl;
    system("PAUSE");
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
            case Server::draw:
                commandDraw(clientId);
                sendUpdateToAll();
                break;
            case Server::discard:
                commandDiscard(clientId);
                sendUpdateToAll();
                break;
            case Server::finish:
                commandFinishTurn(clientId);
                sendUpdateToAll();
                sendTurnInformation();
                break;
            case Server::skip:
                commandMissTurn();
                sendTurnInformation();
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
}

void Server::previousPlayer() {
    if (actualPlayer == 0)
        actualPlayer = numberOfPlayers - 1;
    else
        actualPlayer--;
}

void Server::commandDraw(unsigned int clientId, int b) {
    packet.clear();

    if (b != 0)
        bonus = b;
    else if (bonus == 0)
        bonus = 1;
    packet << ServerConnection::cards;
    packet << bonus;
    for (int i = 0; i < bonus; i++) {
        Card card = deck.getCard();
        packet << card.getColor() << card.getFigure();
        playersCardsCounts[clientId] += 1;
    }
    clients[clientId].send(packet);
    bonus = 0;
}

void Server::commandDiscard(unsigned int clientId) {
    int color, figure;
    packet >> color >> figure;
    cardOnTable = Card(color, figure);
    playersCardsCounts[clientId] -= 1;

    updateGameDataOnDiscard();
    checkVictory(clientId);
}

void Server::sendTurnInformation() {
    packet.clear();
    packet << ServerConnection::move;
    clients[actualPlayer].send(packet);
}

void Server::commandFinishTurn(unsigned int clientId) {
    int turns, figureRequest, colorRequest;
    packet >> turns >> colorRequest >> figureRequest;
    if(request.getFigure() != figureRequest)
        whoRequested = -1;
    request = Card(colorRequest, figureRequest);
    turnsToLose = turns;

    endJackRequest();
    startJackRequest(clientId);
    checkWhoIsNext();
}

void Server::sendUpdateToAll() {
    for (int i = 0; i < numberOfPlayers; i++)
        sendUpdate(i);
}

void Server::sendUpdate(unsigned int clientId) {
    packet.clear();

    packet << ServerConnection::update;

    packet << cardOnTable.getColor() << cardOnTable.getFigure();

    packet << bonus;
    packet << turnsToLose;

    packet << request.getColor() << request.getFigure();

    for (int i = 1; i < maxNumberOfPlayers; i++)
        packet << playersCardsCounts[(clientId + i) % maxNumberOfPlayers];

    clients[clientId].send(packet);
}

void Server::commandMissTurn() {
    if (!moveBack)
        nextPlayer();
    else
        previousPlayer();
}

void Server::sendVictoryInformation(unsigned int clientId) {
    packet.clear();

    packet << ServerConnection::victory;
    packet << win;
    clients[clientId].send(packet);
    win++;
}

void Server::endJackRequest() {
    if (request.getFigure() != Card::noFigure)
        if (whoRequested == actualPlayer) {
            whoRequested = -1;
            request = Card();
            sendUpdateToAll();
        }
}

void Server::startJackRequest(unsigned int clientId) {
    if (request.getFigure() != Card::noFigure)
        if (whoRequested == -1)
            whoRequested = (int) clientId;
}

void Server::checkWhoIsNext() {
    if (cardOnTable.getFigure() == Card::king && cardOnTable.getColor() == Card::spade) {
        previousPlayer();
        moveBack = true;
    } else {
        nextPlayer();
        moveBack = false;
    }
}

void Server::updateGameDataOnDiscard() {
    if (cardOnTable.isBrave())
        bonus += (int) cardOnTable.getPower();
    else if (cardOnTable.getFigure() == Card::four)
        turnsToLose++;
    if (request.getColor() != Card::noColor)
        if (cardOnTable.getColor() == request.getColor())
            request.setColor(Card::noColor);
}

void Server::checkVictory(unsigned int clientId) {
    if (playersCardsCounts[clientId] == 0) {
        playersWhoFinished++;
        sendVictoryInformation(clientId);
    }
    if (playersWhoFinished == numberOfPlayers - 1) {
        playersWhoFinished++;
        nextPlayer();
        sendVictoryInformation(actualPlayer);
    }

}
