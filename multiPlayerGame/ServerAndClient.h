#ifndef __SERVERANDCLIENT_h
#define __SERVERANDCLIENT_h

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Deck.h"
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
		draw, //server ma daæ graczowi okreslona ilosc kart (1 jesli brak "bonusów")
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

class Client
{
public:
	Client(unsigned int serverPort = 22);
	bool welcomeScreen();
	void play();
	enum commands
	{
		move = 0, //informacja o tym, ze kolej na gracza
		cardsList, //arg: ilosc kart (przy rozdaniu, ciagnieciu), kolor, figura, kolor....
		cardOnTableActualization, //kolor, figura
		numbersOfCards, //3 liczby, informuja o ilosci kart innych
		catchBonus, //przyjmuje bonus
		place //przyjmuje informacje o zajêtym miejscu (po zwyciêstwie)
	};
private:
	int myPlace;
	bool moveBack;

	void drawCards();
	void finishTurn();
	void discardMove();
	void showPlayersCards(sf::Vector2i basicCardsPosition, int cardsXChange);
	void showOtherPlayersCards();
	void showCardOnTable();
	void showStatement();
	void showButtons();
	int checkCardsInterval(int numberOfCards);
	bool realizeJackMove();
	bool realizeAceMove();
	bool recieveSomething();
	string enterInformation(string informationName, bool hidden = false);
	void initializeButtons();

	int figureRequest;
	int colorRequest;
	int turnsToLose;
	
	bool itsMyTurn;
	bool waiting;
	bool waitPermanently;
	unsigned int figureOfAlreadyThrown;
	bool somethingWasThrown;
	bool somethingWasDone;
	bool jackWasThrown;
	bool aceWasThrown;
	bool fourWasThrown;
	int bonus;
	
	unsigned int* numberOfOtherCards;
	Card cardOnTable;
	
	sf::TcpSocket server;
	sf::IpAddress serverIp;
	sf::RenderWindow* window;
	unsigned int port;
	sf::Packet packet;
	sf::Packet chatPacket;
	
	sf::Text makeMoveButton;
	sf::Text drawButton;
	sf::Text discardButton;
	sf::Text autor;
	
	PlayerCards cards;
	CardsImages images;

	sf::Font font;
	sf::Color backgroundColor;
};

#endif