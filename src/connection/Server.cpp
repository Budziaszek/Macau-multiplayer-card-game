//
// Created by Magdalena on 18.09.2020.
//

#include "Server.h"
#include "../Client.h"

Server::Server(unsigned int portToListen, unsigned int maxNumberOfPlayers)
{
    whoRequested = -1;
    playersCards = new PlayerCards[maxNumberOfPlayers];
    client = new sf::TcpSocket[maxNumberOfPlayers];
    port = portToListen;
    numberOfPlayers = 0;
    maximumNumberOfPlayers = maxNumberOfPlayers;
    bonus = 0;
    actualPlayer = 0;
    for (int i = 0; i < maximumNumberOfPlayers; i++)
        playersCards[i].numberOfCards = 0;
    win = 1; //pierwsze miejsce do zdobycia
    playersWhoFinished = new bool[numberOfPlayers];
    for (int i = 0; i < numberOfPlayers; i++)
        playersWhoFinished[i] = false;
    numberOfPlayersWhoFinished = 0;
}
bool Server::startListening()
{
    cout << "SERWER" << endl;
    if (listener.listen(port) != sf::Socket::Done)
    {
        cout << "Nie mozna rozpoczac nasluchiwania na porcie: " << port << endl;
        system("PAUSE");
        return false;
    }
    else
    {
        cout << "Nasluchiwanie na porcie: " << port << endl;
        cout << "Serwer publiczne ip: " << sf::IpAddress::getPublicAddress() << endl;
        cout << "Serwer lokalne ip: " << sf::IpAddress::getLocalAddress() << endl;
    }

    return true;
}
void Server::waitForPlayers(unsigned int definedNumberOfPlayers)
{
    cout << "Oczekiwanie na graczy" << endl;
    while (numberOfPlayers < definedNumberOfPlayers)
    {
        listener.accept(client[numberOfPlayers]);
        std::cout << "Polaczyl sie gracz o ip: " << client[numberOfPlayers].getRemoteAddress() << std::endl;
        numberOfPlayers++;
    }
}
void Server::sendCards(unsigned int clientId)
{
    packet.clear();
    packet << Client::cardsList << playersCards[clientId].numberOfCards;
    for (int i = 0; i < playersCards[clientId].numberOfCards; i++)
    {
        packet << playersCards[clientId].checkOne(i).getColor() << playersCards[clientId].checkOne(i).getFigure();
    }
    if (client[clientId].send(packet) == sf::Socket::Done)
        cout << "Wyslano karty do gracza " << clientId << endl;
}
bool Server::recieveSomethingAndResponse(unsigned int clientId)
{
    packet.clear();
    if (client[clientId].receive(packet) == sf::Socket::Done)
    {
        int commandId;
        packet >> commandId;
        if (commandId == Server::draw)
        {

            packet.clear();
            packet << Client::cardsList;
            packet << 1;

            Card card = deck.giveOne();
            packet << card.getColor() << card.getFigure();
            playersCards[clientId].draw(card);

            while(client[clientId].send(packet)!=sf::Socket::Done)
                ;

            for (int i = 0; i < numberOfPlayers; i++)
            {
                if (i != clientId)
                    sendCardOnTable(i);
            }

            for (int i = 0; i < numberOfPlayers; i++)
                if (i != clientId)
                    sendNumbersOfCards(i);
            cout << "     Client " << clientId << " zadanie karty" << endl;
        }
        else if (commandId == Server::discard)
        {
            int color, figure;
            packet >> color >> figure;
            playersCards[clientId].discard(Card(color, figure), deck);
            cardOnTable = Card(color, figure);

            for (int i = 0; i < numberOfPlayers; i++)
            {
                if(i!=clientId)
                    sendCardOnTable(i);
            }
            for (int i = 0; i < numberOfPlayers; i++)
                if (i != clientId)
                    sendNumbersOfCards(i);
            cout << "     Client " << clientId << " wyrzucenie" << endl;
        }
        else if (commandId == Server::finish)
        {
            packet >> bonus;
            cout << "    Client " << clientId << " zakonczenie. Przekazany bonus: " <<bonus<< endl;
            if (bonus == Server::Jack)
            {
                packet >> figureRequest;
                cout << "      Zadanie figury " << figureRequest << endl;
                whoRequested = clientId;
            }
            else if (bonus == Server::Ace)
            {
                packet >> colorRequest;
                cout << "      Zadanie koloru " << colorRequest << endl;
            }
            else if (bonus == Server::Four)
            {
                packet >> turnsToLose;
                cout << "      Do stracenia " << turnsToLose << " kolejek" << endl;
            }
            else if (bonus == Server::continueReqest)
            {
                if (whoRequested != actualPlayer)
                    bonus = Server::Jack;
                if (whoRequested == actualPlayer)
                {
                    bonus = 0;
                    whoRequested = -1;
                }
            }
            moveBack = false;
            packet >> moveBack;
            if (moveBack)
            {
                cout << "          Przejscie do poprzedniego gracza" << endl;
                previousPlayer();
            }
            else
            {
                cout << "          Przejscie do nastepnego gracza" << endl;
                nextPlayer();
            }

            packet.clear();
            packet << Client::move;
            while (client[actualPlayer].send(packet) != sf::Socket::Done);
            packet.clear();
            packet << -1;
            while (client[clientId].send(packet) != sf::Socket::Done);
            cout << "Ruch gracza " << actualPlayer << endl;
        }
        else if (commandId == Server::giveBonus)
        {
            packet.clear();
            packet<< Client::catchBonus << bonus;
            if (bonus == Server::Jack)
                packet << figureRequest;
            else if (bonus == Server::Four)
                packet << turnsToLose;
            else if (bonus == Server::Ace)
                packet << colorRequest;
            packet << moveBack;
            while (client[actualPlayer].send(packet) != sf::Socket::Done);
            cout << "     Client " << clientId << " wysylam bonus " <<bonus << endl;
        }
        else if (commandId == Server::loseTurn)
        {
            cout << "    Client " << clientId << " utrata kolejki." << endl;

            if (moveBack)
            {
                cout << "          Przejscie do poprzedniego gracza" << endl;
                previousPlayer();
            }
            else
            {
                cout << "          Przejscie do nastepnego gracza" << endl;
                nextPlayer();
            }

            packet.clear();
            packet << Client::move;
            while (client[actualPlayer].send(packet) != sf::Socket::Done);
            packet.clear();
            packet << -1;
            while (client[clientId].send(packet) != sf::Socket::Done);
            cout << "Ruch gracza " << actualPlayer << endl;
        }
        else if (commandId == Server::winStatus)
        {
            cout << "    Client " << clientId << " zakonczyl rozgrywke na miejscu " << win << endl;

            packet.clear();
            packet << Client::place << win;
            win++;
            while (client[actualPlayer].send(packet) != sf::Socket::Done);
            playersWhoFinished[actualPlayer] = true;
            numberOfPlayersWhoFinished++;

            if (numberOfPlayersWhoFinished == numberOfPlayers - 1)
            {
                packet.clear();
                packet << Client::place << win;
                win++;
                for (int i = 0; i < numberOfPlayers; i++)
                {
                    if (playersWhoFinished[i] != true)
                    {
                        cout << "    Client " << i << " zakonczyl rozgrywke na miejscu " << win - 1 << endl;
                        while (client[i].send(packet) != sf::Socket::Done);
                        break;
                    }
                }
            }
        }


        else
            cout << "Nieprawidlowa komenda " << commandId << endl;
    }
    if (numberOfPlayersWhoFinished == numberOfPlayers - 1)
        return false;
    else
        return true;
}
void Server::nextPlayer()
{
    if (actualPlayer == numberOfPlayers - 1)
        actualPlayer = 0;
    else
        actualPlayer++;
    if (playersWhoFinished[actualPlayer] == true)
        nextPlayer();
}
void Server::previousPlayer()
{
    if (actualPlayer == 0)
        actualPlayer = numberOfPlayers - 1;
    else
        actualPlayer--;
    if (playersWhoFinished[actualPlayer] == true)
        previousPlayer();
}
void Server::sendCardOnTable(unsigned int clientId)
{
    packet.clear();
    packet << Client::cardOnTableActualization;
    for (int i = 0; i < playersCards[clientId].numberOfCards; i++)
    {
        packet << cardOnTable.getColor() << cardOnTable.getFigure() << bonus;
    }
    while (client[clientId].send(packet) != sf::Socket::Done)
        ;
}
void Server::sendNumbersOfCards(unsigned int clientId)
{
    packet.clear();
    packet << Client::numbersOfCards;
    for(int i = 1; i<numberOfPlayers; i++)
        packet<< playersCards[(clientId + i) % numberOfPlayers].numberOfCards;
    packet << 0 << 0 << 0;
    while (client[clientId].send(packet) != sf::Socket::Done);
}
void Server::game()
{
    deck.shuffle();
    deck.show();
    playersCards = deck.deal(numberOfPlayers, 5);
    do {
        cardOnTable = deck.giveOne();
    } while (cardOnTable.getFigure() == 2 || cardOnTable.getFigure() == 3 || cardOnTable.getFigure() == Card::King || cardOnTable.getFigure() == 4 || cardOnTable.getFigure() == Card::Ace || cardOnTable.getFigure() == Card::Jack);

    for (int i = 0; i < numberOfPlayers; i++)
    {
        sendCards(i);
        sendCardOnTable(i);
        sendNumbersOfCards(i);
    }

    packet.clear();
    packet << Client::move;
    while(client[actualPlayer].send(packet)!=sf::Socket::Done);
    while (recieveSomethingAndResponse(actualPlayer));
    cout << "Wszyscy gracze zakonczyli rozgrywke" << endl;
    listener.close();
    system("PAUSE");
}
