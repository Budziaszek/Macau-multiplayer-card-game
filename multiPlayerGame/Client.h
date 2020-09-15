#ifndef __CLIENT_h
#define __CLIENT_h

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Deck.h"
using namespace std;

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