#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include "Deck.h"
#include "Server.h"
#include "Client.h"

using namespace std;

int main()
{
	srand(time(NULL));
	Client client;
	if (client.welcomeScreen())
	{
		Server server;
		server.startListening();
		int numberOfPlayers;
		do {
			cout << "Podaj liczbe graczy (2-4): ";
		cin >> numberOfPlayers;
		} while (numberOfPlayers > 4 || numberOfPlayers < 2);
		server.waitForPlayers(numberOfPlayers);
		server.game();
	}
	return 0;
}



