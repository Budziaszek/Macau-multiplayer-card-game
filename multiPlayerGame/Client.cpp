#include "Client.h"
#include "Server.h"
#include <Windows.h>

Client::Client(unsigned int serverPort)
{
	myPlace = -1;
	port = serverPort;
	backgroundColor = sf::Color(0, 102, 0, 255);
	font.loadFromFile("font.ttf");
	numberOfOtherCards = new unsigned int[3];
}
bool Client::welcomeScreen()
{
	window = new sf::RenderWindow(sf::VideoMode(800, 600), "Makao", sf::Style::Close);

	autor.setFont(font);
	autor.setCharacterSize(15);
	autor.setFillColor(sf::Color(96, 96, 96));
	autor.setOutlineColor(sf::Color(32, 32, 32));
	autor.setPosition(650, 580);
	autor.setString("Magdalena Budziaszek");

	sf::Event event;
	sf::Text text;
	text.setString("Graj");
	text.setFillColor(sf::Color::White);
	text.setFont(font);
	text.setCharacterSize(50);
	text.setPosition(340, 250);

	while (window->isOpen())
	{
		while (window->pollEvent(event))
		{
			if (text.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
				text.setFillColor(sf::Color::Yellow);
			else
				text.setFillColor(sf::Color::White);

			window->clear(backgroundColor);
			window->draw(text);
			window->draw(autor);
			window->display();

			if (event.type == sf::Event::Closed)
				window->close();
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				if (text.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
				{
					cout << "GRAJ" << endl;
					if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
						FreeConsole();
					while (1)
					{
						string ipString = enterInformation("Wpisz ip serwera: ");
						if (ipString != "")
						{
							serverIp = sf::IpAddress(ipString);
							if (server.connect(serverIp, port) == sf::Socket::Done)
								break;
						}
					}
					cout << "Rozpoczecie gry" << endl;
					window->clear(backgroundColor);
					text.setString("Oczekiwanie na pozostalych graczy");
					text.setCharacterSize(40);
					text.setFillColor(sf::Color::White);
					text.setPosition(100, 250);
					window->draw(text);
					window->display();
					play();
					window->close();
				}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))//server
			{
				window->close();
				return true;
			}
		}
	}
	return false;
}
void Client::play()
{
	moveBack = false;
	itsMyTurn = false;
	somethingWasThrown = false;
	jackWasThrown = false;
	aceWasThrown = false;
	fourWasThrown = false;
	waiting = false;
	bool startPackReceived = false;
	figureOfAlreadyThrown = Card::noFigure;

	sf::SocketSelector selector;
	selector.add(server);

	images.load("cards.png", sf::Vector2u(71, 96));
	sf::Vector2i basicCardsPosition = sf::Vector2i(110, 450);

	initializeButtons();

	while (!startPackReceived)
	{
		if (selector.wait(sf::milliseconds(10)))
		{
			if (selector.isReady(server))
			{
				while (!recieveSomething()); //tu pobiera karty
				cout << "Karty pobrane" << endl;
				while (!recieveSomething()); //karta na stole
				cout << "Karta na stole pobrana" << endl;
				while (!recieveSomething()); //karty innych
				cout << "Karty innych graczy pobrane" << endl;
				startPackReceived = true;
			}
		}
	}

	sf::Event event;
	while (window->isOpen())
	{
		window->clear(backgroundColor);
		int cardsXChange = checkCardsInterval(cards.numberOfCards);
		showPlayersCards(basicCardsPosition, cardsXChange);
		showStatement();
		if (itsMyTurn)
			showButtons();
		showCardOnTable();
		showOtherPlayersCards();
		window->draw(autor);
		window->display();

		while (window->pollEvent(event))
		{
			if (itsMyTurn)
			{
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
				{
					if (sf::Mouse::getPosition(*window).x > basicCardsPosition.x && sf::Mouse::getPosition(*window).x < (basicCardsPosition.x + cards.numberOfCards * cardsXChange))
					{
						if (sf::Mouse::getPosition(*window).y > basicCardsPosition.y && sf::Mouse::getPosition(*window).y < basicCardsPosition.y + 96)
						{
							int whichCard = (sf::Mouse::getPosition(*window).x - basicCardsPosition.x) / cardsXChange;
							if (bonus == Server::Jack)
							{
								if (cards.checkOne(whichCard).getFigure() == figureRequest || cards.checkOne(whichCard).getFigure() == Card::Jack)//tylko do figury lub walet
									cards.changeChoose(whichCard);
							}
							else if (bonus == Server::Four)
							{
								if (cards.checkOne(whichCard).getFigure() == 4 && !waiting)
									cards.changeChoose(whichCard);
							}
							else if (bonus == Server::Ace)
							{
								if (cards.checkOne(whichCard).getColor() == colorRequest || cards.checkOne(whichCard).getFigure() == Card::Ace) //tylko zadany kolor lub as
									cards.changeChoose(whichCard);
							}
							else if (!waiting)
							{
								if (cards.checkOne(whichCard).getColor() == cardOnTable.getColor() || cards.checkOne(whichCard).getFigure() == cardOnTable.getFigure())//tylko do koloru lub figury
									if (figureOfAlreadyThrown == 0 || figureOfAlreadyThrown == cards.checkOne(whichCard).getFigure()) //w jednym ruchu tylko te same figury
										if (bonus <= 0 || (cards.checkOne(whichCard).getFigure() == 2 || cards.checkOne(whichCard).getFigure() == 3 || (cards.checkOne(whichCard).getFigure() == Card::King && (cards.checkOne(whichCard).getColor() == Card::heart || cards.checkOne(whichCard).getColor() == Card::spade)))) //waleczna na waleczna
											cards.changeChoose(whichCard);
							}
						}
					}

					if (drawButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))) && somethingWasThrown == false && bonus != Server::Four && !waiting)
					{
						drawCards();
						finishTurn();
					}
					if (makeMoveButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))) && (somethingWasDone || bonus == Server::Four || waiting))
					{
						if (!fourWasThrown && turnsToLose > 0)
							waiting = true;
						else
						{
							if (jackWasThrown)
								if (realizeJackMove())
									bonus = Server::Jack;
							if (aceWasThrown)
								if (realizeAceMove())
									bonus = Server::Ace;
						}
						finishTurn();

					}
					if (discardButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))) && !waiting)
						discardMove();
				}
			}
			if (event.type == sf::Event::Closed)
				window->close();
			if (!itsMyTurn)
			{
				if (selector.wait(sf::milliseconds(10)))
				{
					if (selector.isReady(server))
					{
						recieveSomething();
						if (itsMyTurn && !waitPermanently)
						{
							packet.clear();
							packet << Server::giveBonus;
							server.send(packet);
							recieveSomething();
						}
					}
				}

			}
		}
	}
	server.disconnect();
}
void Client::showPlayersCards(sf::Vector2i basicCardsPosition, int cardsXChange)
{
	images.setPosition(sf::Vector2i(basicCardsPosition.x, basicCardsPosition.y)); //delete if works without
	int cardsYChange = checkCardsInterval(cards.numberOfCards);

	for (int i = 0; i < cards.numberOfCards; i++)
	{
		images.setNumber((cards.checkOne(i).getColor() - 1) * 13 + cards.checkOne(i).getFigure() - 1);
		if (cards.checkOne(i).chosen == true && itsMyTurn)
		{
			images.setPosition(sf::Vector2i(basicCardsPosition.x + i * cardsXChange, basicCardsPosition.y - cardsYChange));
		}
		else
			images.setPosition(sf::Vector2i(basicCardsPosition.x + i * cardsXChange, basicCardsPosition.y));
		window->draw(images);
	}
}
void Client::showCardOnTable()
{
	images.setNumber((cardOnTable.getColor() - 1) * 13 + cardOnTable.getFigure() - 1);
	images.setPosition(sf::Vector2i(400 - 35, 300 - 48));
	window->draw(images);
}
void Client::showOtherPlayersCards()
{
	sf::RectangleShape cardDown(sf::Vector2f(71, 96));
	cardDown.setFillColor(sf::Color(153, 0, 0, 255));
	cardDown.setOutlineThickness(-1);
	cardDown.setOutlineColor(sf::Color::Black);

	int cardsXChange = checkCardsInterval(numberOfOtherCards[0]);

	cardDown.setRotation(90);
	for (int i = 0; i < numberOfOtherCards[0]; i++)//wyswietl karty gracza po lewej
	{
		cardDown.setPosition(sf::Vector2f(100, 50 + i * cardsXChange));
		window->draw(cardDown);
	}

	cardsXChange = checkCardsInterval(numberOfOtherCards[1]);

	cardDown.setRotation(0);
	for (int i = 0; i < numberOfOtherCards[1]; i++)//wyswietl karty gracza u gory
	{
		cardDown.setPosition(sf::Vector2f(150 + i * cardsXChange, 50));
		window->draw(cardDown);
	}

	cardsXChange = checkCardsInterval(numberOfOtherCards[2]);

	cardDown.setRotation(90);
	for (int i = 0; i < numberOfOtherCards[2]; i++)//wyswietl karty z prawej
	{
		cardDown.setPosition(sf::Vector2f(800 - 50, 600 - 150 - i * cardsXChange));
		window->draw(cardDown);
	}

}
void Client::showStatement()
{
	sf::Text bonusToTake;
	bonusToTake.setFont(font);
	bonusToTake.setCharacterSize(15);
	bonusToTake.setPosition(200, 400);

	string communicatToTake = "Kart do wziecia: ";
	string communicatRequest = "Zadanie: ";
	string communicatWait = "Kolejki do odstania: ";
	string communicatWin = "Gratulacje. Zajales miejsce ";
	string communicatLose = "Przegrales! Zajales miejsce ";

	if (bonus > 0)
	{
		bonusToTake.setString(communicatToTake + std::to_string(bonus));
		window->draw(bonusToTake);
	}
	if (bonus == Server::Jack)
	{
		if (figureRequest == Card::Queen)
			bonusToTake.setString(communicatRequest + "Q");
		else
			bonusToTake.setString(communicatRequest + std::to_string(figureRequest));
		window->draw(bonusToTake);
	}
	if (bonus == Server::Ace)
	{
		string colorStringRequest;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		{
			if (colorRequest == 1)
				colorStringRequest = "trefl (koniczyna)";
			if (colorRequest == 2)
				colorStringRequest = "kier (serce)";
			if (colorRequest == 3)
				colorStringRequest = "pik (lisc)";
			if (colorRequest == 4)
				colorStringRequest = "karo (diament)";
		}
		else
		{
			if (colorRequest == 1)
				colorStringRequest = "trefl";
			if (colorRequest == 2)
				colorStringRequest = "kier";
			if (colorRequest == 3)
				colorStringRequest = "pik";
			if (colorRequest == 4)
				colorStringRequest = "karo";
		}

		bonusToTake.setString(communicatRequest + colorStringRequest);
		window->draw(bonusToTake);
	}
	if (turnsToLose > 0)
	{
		bonusToTake.setString(communicatWait + std::to_string(turnsToLose));
		window->draw(bonusToTake);
	}
	if (waitPermanently)
	{
		if (cards.numberOfCards > 0)
			bonusToTake.setString(communicatLose + std::to_string(myPlace));
		else
			bonusToTake.setString(communicatWin + std::to_string(myPlace));
		window->draw(bonusToTake);
	}
}
void Client::showButtons()
{
	if (waitPermanently)
		return;
	if (drawButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
		drawButton.setFillColor(sf::Color::Yellow);
	else
		drawButton.setFillColor(sf::Color::White);

	if (makeMoveButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
		makeMoveButton.setFillColor(sf::Color::Yellow);
	else
		makeMoveButton.setFillColor(sf::Color::White);

	if (discardButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
		discardButton.setFillColor(sf::Color::Yellow);
	else
		discardButton.setFillColor(sf::Color::White);

	if (somethingWasDone || waiting || bonus == Server::Four)
		window->draw(makeMoveButton);
	if (!waiting)
		window->draw(discardButton);
	if (!somethingWasThrown && bonus != Server::Four && !waiting)
		window->draw(drawButton);
}
void Client::drawCards()
{
	//ciagnij wiele
	if (bonus > 0)
		for (int i = 0; i < bonus; i++)
		{
			packet.clear();
			packet << Server::draw;
			while (server.send(packet) != sf::Socket::Done)
				;
			recieveSomething();
		}
	else //ciagnij jedna
	{
		packet.clear();
		packet << Server::draw;
		while (server.send(packet) != sf::Socket::Done)
			;
		recieveSomething();
	}
	if (bonus > 0)
		bonus = 0;
}
void Client::discardMove()
{
	somethingWasThrown = true;
	Card card = cards.discard();

	if (card.getFigure() != Card::noFigure && card.getColor() != Card::noColor)
	{
		if (bonus < 0 && bonus != Server::continueReqest && bonus != Server::Jack)
			bonus = 0;
		if (card.getFigure() == 2 || card.getFigure() == 3)
			bonus += card.getFigure();
		else if (card.getFigure() == Card::King && card.getColor() == Card::heart)
		{
			bonus += 5;
			moveBack = false;
		}
		else if (card.getFigure() == Card::King && card.getColor() == Card::spade)
		{
			bonus += 5;
			moveBack = true;
		}
		else if (card.getFigure() == Card::Jack)
			jackWasThrown = true;
		else if (card.getFigure() == Card::Ace)
			aceWasThrown = true;
		else if (card.getFigure() == Card::Four)
		{
			turnsToLose++;
			fourWasThrown = true;
			waiting = false;
		}

		cardOnTable = card;
		packet.clear();
		packet << Server::discard << card.getColor() << card.getFigure();
		figureOfAlreadyThrown = card.getFigure();
		while (server.send(packet) != sf::Socket::Done)
			;
		somethingWasDone = true;
		if (bonus == Server::Ace)
			bonus = 0;
	}
}
void Client::finishTurn()
{
	if (!waiting)
	{

		if (cards.numberOfCards == 0)
		{
			packet.clear();
			packet << Server::winStatus;
			while (server.send(packet) != sf::Socket::Done)
				;
			recieveSomething();
			waitPermanently = true;
		}

		packet.clear();
		packet << Server::finish;

		if (!jackWasThrown && bonus == Server::Jack)
			packet << Server::continueReqest;
		else if (fourWasThrown)
			packet << Server::Four << turnsToLose;
		else
			packet << bonus;
		if (jackWasThrown)
			packet << figureRequest;
		if (aceWasThrown || bonus == Server::Ace)
			packet << colorRequest;
		packet << moveBack;

		while (server.send(packet) != sf::Socket::Done)
			;

		if (bonus == Server::Four && fourWasThrown)
			turnsToLose = 0;
		bonus = 0;
		turnsToLose = 0;
		moveBack = false;
		jackWasThrown = false;
		aceWasThrown = false;
		fourWasThrown = false;
		itsMyTurn = false;
		somethingWasThrown = false;
		figureOfAlreadyThrown = Card::noFigure;
		somethingWasDone = false;

		cards.resetChoosen();
		cout << "Zakonczono kolejke" << endl;
	}
	else
	{
		if (bonus == Server::wait)
		{
			packet.clear();
			packet << Server::loseTurn;
			while (server.send(packet) != sf::Socket::Done)
				;
			itsMyTurn = false;
			somethingWasDone = false;
			cout << "Kolejka zakonczona (utrata) - kolejna" << endl;
		}
		else
		{
			packet.clear();
			packet << Server::finish << 0 << 0;

			while (server.send(packet) != sf::Socket::Done)
				;
			bonus = Server::wait;
			itsMyTurn = false;
			somethingWasDone = false;
			cout << "Kolejka zakonczona (utrata) - pierwsza" << endl;
		}
		turnsToLose--;
		if (turnsToLose == 0)
			waiting = false;
	}
}
bool Client::realizeJackMove()
{
	sf::RenderWindow* miniWindow = new sf::RenderWindow(sf::VideoMode(200, 200), "Zadanie (Walet)", sf::Style::Close);

	sf::Event event;
	sf::Text tak, nie;
	tak.setString("Tak");
	tak.setFillColor(sf::Color::White);
	tak.setFont(font);
	tak.setCharacterSize(20);
	tak.setPosition(50, 100);

	nie.setString("Nie");
	nie.setFillColor(sf::Color::White);
	nie.setFont(font);
	nie.setCharacterSize(20);
	nie.setPosition(150, 100);

	sf::Text information;
	information.setString("Czy chcesz zazadac figury?");
	information.setFillColor(sf::Color::White);
	information.setFont(font);
	information.setCharacterSize(15);
	information.setPosition(10, 20);

	while (miniWindow->isOpen())
	{
		while (miniWindow->pollEvent(event))
		{
			if (tak.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
				tak.setFillColor(sf::Color::Yellow);
			else
				tak.setFillColor(sf::Color::White);

			if (nie.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
				nie.setFillColor(sf::Color::Yellow);
			else
				nie.setFillColor(sf::Color::White);

			miniWindow->clear(backgroundColor);
			miniWindow->draw(tak);
			miniWindow->draw(nie);
			miniWindow->draw(information);
			miniWindow->display();

			if (event.type == sf::Event::Closed)
			{
				miniWindow->close();
				return false;
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				if (nie.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
				{
					miniWindow->close();
					return false;
				}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				if (tak.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
				{

					sf::Text tab[7];
					tab[0].setString("5");
					tab[1].setString("6");
					tab[2].setString("7");
					tab[3].setString("8");
					tab[4].setString("9");
					tab[5].setString("10");
					tab[6].setString("Q");

					for (int i = 0; i < 7; i++)
					{
						tab[i].setFillColor(sf::Color::White);
						tab[i].setFont(font);
						tab[i].setCharacterSize(20);
						tab[i].setPosition(50, 20 + i * 15);
					}

					while (miniWindow->isOpen())
					{
						while (miniWindow->pollEvent(event))
						{
							for (int i = 0; i < 7; i++)
							{

								if (tab[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
									tab[i].setFillColor(sf::Color::Yellow);
								else
									tab[i].setFillColor(sf::Color::White);
							}
							for (int i = 0; i < 7; i++)
							{
								if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
									if (tab[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
									{
										if (i == 6)
											figureRequest = Card::Queen;
										else
											figureRequest = i + 5;
										miniWindow->close();
									}
							}

							miniWindow->clear(backgroundColor);
							for (int i = 0; i < 7; i++)
								miniWindow->draw(tab[i]);
							miniWindow->display();
						}
					}

					return true;
				}
		}
	}
	return false;
}
bool Client::realizeAceMove()
{
	sf::RenderWindow* miniWindow = new sf::RenderWindow(sf::VideoMode(200, 200), "Zadanie (As)", sf::Style::Close);

	sf::Event event;
	sf::Text tak, nie;
	tak.setString("Tak");
	tak.setFillColor(sf::Color::White);
	tak.setFont(font);
	tak.setCharacterSize(20);
	tak.setPosition(50, 100);

	nie.setString("Nie");
	nie.setFillColor(sf::Color::White);
	nie.setFont(font);
	nie.setCharacterSize(20);
	nie.setPosition(150, 100);

	sf::Text information;
	information.setString("Czy chcesz zazadac koloru?");
	information.setFillColor(sf::Color::White);
	information.setFont(font);
	information.setCharacterSize(15);
	information.setPosition(10, 20);

	while (miniWindow->isOpen())
	{
		while (miniWindow->pollEvent(event))
		{
			if (tak.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
				tak.setFillColor(sf::Color::Yellow);
			else
				tak.setFillColor(sf::Color::White);

			if (nie.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
				nie.setFillColor(sf::Color::Yellow);
			else
				nie.setFillColor(sf::Color::White);

			miniWindow->clear(backgroundColor);
			miniWindow->draw(tak);
			miniWindow->draw(nie);
			miniWindow->draw(information);
			miniWindow->display();

			if (event.type == sf::Event::Closed)
			{
				miniWindow->close();
				return false;
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				if (nie.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
				{
					miniWindow->close();
					return false;
				}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				if (tak.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
				{

					sf::Text tab[4];
					tab[0].setString("trefl");
					tab[1].setString("kier ");
					tab[2].setString("pik ");
					tab[3].setString("karo");

					sf::Text tabBig[4];
					tabBig[0].setString("trefl (koniczyna)");
					tabBig[1].setString("kier (serce)");
					tabBig[2].setString("pik (lisc)");
					tabBig[3].setString("karo (diament)");

					for (int i = 0; i < 4; i++)
					{
						tab[i].setFillColor(sf::Color::White);
						tab[i].setFont(font);
						tab[i].setCharacterSize(20);
						tab[i].setPosition(45, 30 + i * 15);

						tabBig[i].setFillColor(sf::Color::White);
						tabBig[i].setFont(font);
						tabBig[i].setCharacterSize(20);
						tabBig[i].setPosition(45, 30 + i * 15);
					}

					while (miniWindow->isOpen())
					{
						while (miniWindow->pollEvent(event))
						{
							for (int i = 0; i < 4; i++)
							{

								if (tab[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
								{
									tab[i].setFillColor(sf::Color::Yellow);
									tabBig[i].setFillColor(sf::Color::Yellow);
								}
								else
								{
									tab[i].setFillColor(sf::Color::White);
									tabBig[i].setFillColor(sf::Color::Yellow);
								}
							}
							for (int i = 0; i < 4; i++)
							{
								if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
									if (tab[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*miniWindow))))
									{
										colorRequest = i + 1;
										miniWindow->close();
									}
							}

							miniWindow->clear(backgroundColor);
							for (int i = 0; i < 4; i++)
							{
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
									miniWindow->draw(tabBig[i]);
								else
									miniWindow->draw(tab[i]);
							}
							miniWindow->display();
						}
					}

					return true;
				}
		}
	}
	return false;
}
int Client::checkCardsInterval(int numberOfCards)
{
	if (numberOfCards <= 8)
		return 50;
	if (numberOfCards < 16)
		return 30;
	if (numberOfCards < 24)
		return 20;
	if (numberOfCards < 32)
		return 10;
	else
		return 5;
}
bool Client::recieveSomething()
{
	packet.clear();
	if (server.receive(packet) == sf::Socket::Done)
	{
		int commandId;
		packet >> commandId;
		if (waitPermanently)
		{
			packet.clear();
			packet << Server::loseTurn;
			while (server.send(packet) != sf::Socket::Done)
				;
			itsMyTurn = false;
			somethingWasDone = false;
			cout << "Kolejka opuszczona z powodu zakonczenia gry." << endl;
		}
		if (commandId == Client::cardsList)
		{
			int numberOfCardsToTake;
			packet >> numberOfCardsToTake;
			for (int i = 0; i < numberOfCardsToTake; i++)
			{
				int color, figure;
				packet >> color >> figure;
				Card card = Card(color, figure);
				cards.draw(card);
			}
			return true;
			cout << "Otrzymano karty" << endl;
		}
		else if (commandId == Client::cardOnTableActualization)
		{
			int color, figure;
			packet >> color >> figure;
			cardOnTable = Card(color, figure);
			cout << "Nowa karta na stole" << endl;
		}
		else if (commandId == Client::move)
		{
			itsMyTurn = true;
			cout << "Moj ruch" << endl;
		}
		else if (commandId == Client::numbersOfCards)
		{
			packet >> numberOfOtherCards[0] >> numberOfOtherCards[1] >> numberOfOtherCards[2];
			cout << "Zmiana ilosci kart innnych graczy! " << numberOfOtherCards[0] << " " << numberOfOtherCards[1] << " " << numberOfOtherCards[2] << endl;
		}
		else if (commandId == Client::catchBonus)
		{
			if (!waiting)
			{
				packet >> bonus;
				cout << "Aktualny bonus: " << bonus << endl;
				if (bonus == Server::Jack)
					packet >> figureRequest;
				else if (bonus == Server::Four)
					packet >> turnsToLose;
				else if (bonus == Server::Ace)
					packet >> colorRequest;
			}
			if (waiting)
			{
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
		}
		else if (commandId == Client::place)
		{
			packet >> myPlace;
			cout << "Zajeto " << myPlace << " miejsce" << endl;
			itsMyTurn = true;
			waitPermanently = true;
		}

		return true;
	}
	else
		return false;
}
void Client::initializeButtons()
{
	discardButton.setFont(font);
	discardButton.setString("Rzuc");
	discardButton.setCharacterSize(20);
	discardButton.setPosition(130, 550);

	drawButton.setFont(font);
	drawButton.setString("Ciagnij");
	drawButton.setCharacterSize(20);
	drawButton.setPosition(250, 550);

	makeMoveButton.setFont(font);
	makeMoveButton.setString("Zakoncz ruch");
	makeMoveButton.setCharacterSize(20);
	makeMoveButton.setPosition(370, 550);
}
string Client::enterInformation(string informationName, bool hidden)
{
	std::string information = "";
	sf::Event event;
	sf::Text text;
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(30);
	text.setPosition(50, 100);

	while (window->isOpen())
	{
		while (window->pollEvent(event))
		{
			text.setString(informationName);
			window->clear(backgroundColor);
			window->draw(text);
			window->draw(autor);
			window->display();

			if (event.type == sf::Event::TextEntered && event.key.code != sf::Keyboard::Return)
			{
				if (event.text.unicode == '\b' && information.length() > 0)
				{
					information.erase(information.size() - 1, 1);
					informationName.erase(informationName.size() - 1, 1);
				}
				else if (event.key.code != sf::Keyboard::Return)
				{
					information += static_cast<char>(event.text.unicode);
					if (hidden == false)
						informationName += static_cast<char>(event.text.unicode);
					else
						informationName += "*";
				}
			}
			if (event.type == sf::Event::Closed)
			{
				window->close();
				return "";
			}

			if (event.key.code == sf::Keyboard::Return && information.length() > 0)
				return information;
		}
	}
	return "";
}