//
// Created by Magdalena on 19.09.2020.
//

#include "ServerConnection.h"
#include "Server.h"

ServerConnection::ServerConnection(unsigned int serverPort) {
    port = serverPort;
}

bool ServerConnection::connect(const string &ip) {
    serverIp = sf::IpAddress(ip);
    if (server.connect(serverIp, port) == sf::Socket::Done) {
        selector.add(server);
        return true;
    }
    return false;
}

bool ServerConnection::receiveStartPacket(GameStatus *gameStatus) {
    if (selector.wait(sf::milliseconds(10))) {
        if (selector.isReady(server)) {
            while (!receiveSomething(gameStatus)); //tu pobiera karty
            while (!receiveSomething(gameStatus)); //karta na stole
            while (!receiveSomething(gameStatus)); //karty innych
            return true;
        }
    }
    return false;
}

void ServerConnection::disconnect() {
    server.disconnect();
}

void ServerConnection::checkTurn(GameStatus *gameStatus) {
    if (selector.wait(sf::milliseconds(10))) {
        if (selector.isReady(server)) {
            receiveSomething(gameStatus);
            if (gameStatus->isItsMyTurn() && !gameStatus->isWaitPermanently()) {
                packet.clear();
                packet << Server::giveBonus;
                server.send(packet);
                receiveSomething(gameStatus);
            }
        }
    }
}

bool ServerConnection::receiveSomething(GameStatus *gameStatus) {
    packet.clear();
    if (server.receive(packet) == sf::Socket::Done) {
        int commandId;
        packet >> commandId;
        if (gameStatus->isWaitPermanently()) {
            packet.clear();
            packet << Server::loseTurn;
            while (server.send(packet) != sf::Socket::Done);
            gameStatus->setItsMyTurn(false);
            gameStatus->setSomethingWasDone(false);
            cout << "Kolejka opuszczona z powodu zakonczenia gry." << endl;
        }
        if (commandId == ServerConnection::cardsList) {
            int numberOfCardsToTake;
            packet >> numberOfCardsToTake;
            for (int i = 0; i < numberOfCardsToTake; i++) {
                int color, figure;
                packet >> color >> figure;
                Card card = Card(color, figure);
                gameStatus->draw(card);
            }
            return true;
            cout << "Otrzymano karty" << endl;
        } else if (commandId == ServerConnection::cardOnTableActualization) {
            int color, figure;
            packet >> color >> figure;
            gameStatus->setCardOnTable(Card(color, figure));
            cout << "Nowa karta na stole" << endl;
        } else if (commandId == ServerConnection::move) {
            gameStatus->setItsMyTurn(true);
            cout << "Moj ruch" << endl;
        } else if (commandId == ServerConnection::numbersOfCards) {
            unsigned int n1, n2, n3;
            packet >> n1 >> n2 >> n3;
            unsigned int numbers[] = {n1, n2, n3};
            gameStatus->setNumberOfOtherCards(numbers);
        } else if (commandId == catchBonus) {
            if (!gameStatus->isWaiting()) {
                int bonus = 0;
                packet >> bonus;
                gameStatus->setBonus(bonus);
                cout << "Aktualny bonus: " << gameStatus->getBonus() << endl;
                bool figureRequest, turnsToLose, colorRequest;
                if (bonus == Server::Jack)
                    packet >> figureRequest;
                else if (bonus == Server::Four)
                    packet >> turnsToLose;
                else if (bonus == Server::Ace)
                    packet >> colorRequest;
                gameStatus->setFigureRequest(figureRequest);
                gameStatus->setTurnsToLose(turnsToLose);
                gameStatus->setColorRequest(colorRequest);
            }
            if (gameStatus->isWaiting()) {
                int Pbonus, PfigureRequest, PturnsToLose, PcolorRequest, PmoveBack;

                packet >> Pbonus;
                if (Pbonus == Server::Jack)
                    packet >> PfigureRequest;
                else if (Pbonus == Server::Four)
                    packet >> PturnsToLose;
                else if (Pbonus == Server::Ace)
                    packet >> PcolorRequest;
                packet >> PmoveBack;

                packet.clear();
                packet << Server::finish;

                if (Pbonus == Server::Jack)
                    packet << Server::continueReqest;
                else if (Pbonus == Server::Four)
                    packet << Server::Four << PturnsToLose;
                else
                    packet << Pbonus;
                if (Pbonus == Server::Jack)
                    packet << PfigureRequest;
                if (Pbonus == Server::Ace)
                    packet << PcolorRequest;
                packet << PmoveBack;
            }
        } else if (commandId == place) {
            int myPlace;
            packet >> myPlace;
            gameStatus->setMyPlace(myPlace);
            cout << "Zajeto " << myPlace << " miejsce" << endl;
            gameStatus->setItsMyTurn(true);
            gameStatus->setWaitPermanently(true);
        }

        return true;
    } else
        return false;
}

void ServerConnection::drawCards(GameStatus *gameStatus) {
    if (!gameStatus->getBonus())
        gameStatus->setBonus(gameStatus->getBonus() + 1);
    for (int i = 0; i < gameStatus->getBonus(); i++) {
        packet.clear();
        packet << Server::draw;
        while (server.send(packet) != sf::Socket::Done);
        receiveSomething(gameStatus);
    }
}

void ServerConnection::discard(Card card) {
    packet.clear();
    packet << Server::discard << card.getColor() << card.getFigure();
    while (server.send(packet) != sf::Socket::Done);
}

void ServerConnection::victory(GameStatus *gameStatus) {
    packet.clear();
    packet << Server::winStatus;
    while (server.send(packet) != sf::Socket::Done);
    receiveSomething(gameStatus);
}

void ServerConnection::finishTurn(GameStatus *gameStatus) {
    packet.clear();
    packet << Server::finish;

    if (!gameStatus->isJackWasThrown() && gameStatus->getBonus() == Server::Jack)
        packet << Server::continueReqest;
    else if (gameStatus->isFourWasThrown())
        packet << Server::Four << gameStatus->getTurnsToLose();
    else
        packet << gameStatus->getBonus();
    if (gameStatus->isJackWasThrown())
        packet << gameStatus->getFigureRequest();
    if (gameStatus->isAceWasThrown() || gameStatus->getBonus() == Server::Ace)
        packet << gameStatus->getColorRequest();
    packet << gameStatus->isMoveBack();

    while (server.send(packet) != sf::Socket::Done);

}

void ServerConnection::loseTurn() {
    packet.clear();
    packet << Server::loseTurn;
    while (server.send(packet) != sf::Socket::Done);
}

void ServerConnection::finish() {
    packet.clear();
    packet << Server::finish << 0 << 0;
    while (server.send(packet) != sf::Socket::Done);
}

